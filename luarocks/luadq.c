

#include <assert.h>
#include <string.h>

#define LUA_LIB
#include "lua.h"
#include "lauxlib.h"

#include <dq/dq.h>
#include <dq/vec3.h>
#include <dq/mat3.h>
#include <dq/homo.h>


/*
 * Generic.
 */
static int lua_reg_metatable( lua_State *L, const luaL_reg *reg, const char *name );
static int lua_is_foo( lua_State *L, int ind, const char *foo );


/*
 * Helpers.
 */
static int luaL_checkvec3( lua_State *L, double v[3], int ind );
static int luaL_checkmatrix( lua_State *L, double *M, int rows, int columns, int ind );
static int luaL_checkmat3( lua_State *L, double M[3][3], int ind );
static int lua_pushvec3( lua_State *L, const double v[3] );
static int lua_pushmat3( lua_State *L, double M[3][3] );


/*
 * DQ API
 */
typedef struct dqL_s {
   dq_t dq;
} dqL_t;
#define DQ_METATABLE    "luadq"
/* Internal API. */
static int lua_loaddq( lua_State *L );
static int lua_isdq( lua_State *L, int ind );
static dqL_t* lua_todq( lua_State *L, int ind );
static dqL_t* luaL_checkdq( lua_State *L, int ind );
static dqL_t* lua_pushdq( lua_State *L, dqL_t dq );
#define DQL( n )     static int dqL_##n( lua_State *L )
/* Creation functions. */
DQL( cr_raw );
DQL( cr_rotation );
DQL( cr_rotation_plucker );
DQL( cr_rotation_matrix );
DQL( cr_translation );
DQL( cr_translation_vector );
DQL( cr_point );
DQL( cr_line );
DQL( cr_line_plucker );
DQL( cr_homo );
DQL( cr_copy );
DQL( cr_conj );
DQL( cr_inv );
/* Operation functions. */
DQL( op_norm2 );
DQL( op_add );
DQL( op_sub );
DQL( op_mul );
DQL( op_sign );
DQL( op_f1g );
DQL( op_f2g );
DQL( op_f3g );
DQL( op_f4g );
DQL( op_extract );
/* Check functions. */
DQL( ch_get );
DQL( ch_unit );
DQL( ch_cmp );
/* Misc functions. */
DQL( print );
/* Metatable. */
#undef DQL
static const luaL_reg dqL_methods[] = {
   /* Creation. */
   { "raw", dqL_cr_raw },
   { "rotation", dqL_cr_rotation },
   { "rotation_plucker", dqL_cr_rotation_plucker },
   { "rotation_matrix", dqL_cr_rotation_matrix },
   { "translation", dqL_cr_translation },
   { "translation_vector", dqL_cr_translation_vector },
   { "point", dqL_cr_point },
   { "line", dqL_cr_line },
   { "line_plucker", dqL_cr_line_plucker },
   { "homo", dqL_cr_homo },
   { "copy", dqL_cr_copy },
   { "conj", dqL_cr_conj },
   { "inv", dqL_cr_inv },
   /* Operation. */
   { "norm2", dqL_op_norm2 },
   { "add", dqL_op_add },
   { "__add", dqL_op_add },
   { "sub", dqL_op_sub },
   { "__sub", dqL_op_sub },
   { "mul", dqL_op_mul },
   { "__mul", dqL_op_mul },
   { "sign", dqL_op_sign },
   { "f1g", dqL_op_f1g },
   { "f2g", dqL_op_f2g },
   { "f3g", dqL_op_f3g },
   { "f4g", dqL_op_f4g },
   { "extract", dqL_op_extract },
   /* Check. */
   { "get", dqL_ch_get },
   { "unit", dqL_ch_unit },
   { "cmp", dqL_ch_cmp },
   /* Misc. */
   { "print", dqL_print },
   { 0, 0 }
};


int luaopen_luadq( lua_State *L )
{
   lua_loaddq( L );
   return 0;
}


/*
 * Generic.
 */
static int lua_reg_metatable( lua_State *L, const luaL_reg *reg, const char *name )
{
   luaL_newmetatable(   L, name );
   lua_pushvalue(       L, -1 );
   lua_setfield(        L, -2,   "__index" );
   luaL_register(       L, NULL, reg );
   lua_setfield(        L,       LUA_GLOBALSINDEX, name );
   return 0;
}
static int lua_is_foo( lua_State *L, int ind, const char *foo )
{
   int ret;
   if (lua_getmetatable(L,ind)==0)
      return 0;
   lua_getfield(L, LUA_REGISTRYINDEX, foo);
   ret = 0;
   if (lua_rawequal(L, -1, -2)) /* does it have the correct mt? */
      ret = 1;
   lua_pop(L, 2); /* remove both metatables */
   return ret;
}


/*
 * Helper.
 */
#define TBL_GETNUM(L,v,n,ind) \
lua_pushnumber(L,n); \
lua_gettable(L,ind); \
v = luaL_checknumber(L,-1); \
lua_pop(L,1)
static int luaL_checkvec3( lua_State *L, double v[3], int ind )
{
   int i;
   luaL_checktype(L,ind,LUA_TTABLE);
   for (i=0; i<3; i++) {
      TBL_GETNUM( L, v[i], i+1, ind );
   }
   return 0;
}
static int luaL_checkmatrix( lua_State *L, double *M, int rows, int columns, int ind )
{
   int i, j;
   luaL_checktype(L,ind,LUA_TTABLE);
   for (i=0; i<rows; i++) {
      /* Get row. */
      lua_pushnumber(L,i+1);
      lua_gettable(L,ind);
      luaL_checktype(L,-1,LUA_TTABLE);

      /* Process row. */
      for (j=0; j<columns; j++) {
         TBL_GETNUM( L, M[i*rows+j], j+1, -2 );
      }

      /* Clean up. */
      lua_pop(L,1);
   }
   return 0;
}
static int luaL_checkmat3( lua_State *L, double M[3][3], int ind )
{
   return luaL_checkmatrix( L, (double*)M, 3, 3, ind );
}
static int lua_pushvec3( lua_State *L, const double v[3] )
{
   int i;
   lua_newtable(L);
   for (i=0; i<3; i++) {
      lua_pushnumber( L, i+1 );
      lua_pushnumber( L, v[i] );
      lua_settable( L, -3 );
   }
   return 0;
}
static int lua_pushmat3( lua_State *L, double M[3][3] )
{
   int i, j;
   lua_newtable(L);
   for (j=0; j<3; j++) {
      lua_pushnumber( L, j+1 );
      lua_newtable(L);
      for (i=0; i<3; i++) {
         lua_pushnumber( L, i+1 );
         lua_pushnumber( L, M[j][i] );
         lua_settable( L, -3 );
      }
      lua_settable( L, -3 );
   }
   return 0;
}


/*
 * DQ.
 */
/* Internal API. */
static int lua_loaddq( lua_State *L )
{
   return lua_reg_metatable( L, dqL_methods, DQ_METATABLE );
}
static int lua_isdq( lua_State *L, int ind )
{
   return lua_is_foo( L, ind, DQ_METATABLE );
}
static dqL_t* lua_todq( lua_State *L, int ind )
{
   return (dqL_t*) lua_touserdata( L, ind );
}
static dqL_t* luaL_checkdq( lua_State *L, int ind )
{
   if (lua_isdq( L, ind ))
      return lua_todq( L, ind );
   luaL_typerror( L, ind, DQ_METATABLE );
   return NULL;
}
static dqL_t* lua_pushdq( lua_State *L, dqL_t dq )
{
   dqL_t *dqL;
   dqL = (dqL_t*) lua_newuserdata( L, sizeof(dqL_t) );
   *dqL = dq;
   assert( dqL != NULL );
   luaL_getmetatable( L, DQ_METATABLE );
   lua_setmetatable( L, -2 );
   return dqL;
}
/* Creation API. */
#define DQL_CR_V( n ) \
static int dqL_##n( lua_State *L ) { \
   dqL_t Q; double v[3]; \
   luaL_checkvec3( L, v, 1 ); \
   dq_##n( Q.dq, v ); \
   lua_pushdq( L, Q ); \
   return 1; }
#define DQL_CR_VV( n ) \
static int dqL_##n( lua_State *L ) { \
   dqL_t Q; double v[3], u[3]; \
   luaL_checkvec3( L, v, 1 ); \
   luaL_checkvec3( L, u, 2 ); \
   dq_##n( Q.dq, v, u ); \
   lua_pushdq( L, Q ); \
   return 1; }
#define DQL_CR_DV( n ) \
static int dqL_##n( lua_State *L ) { \
   dqL_t Q; double d, v[3]; \
   d = luaL_checknumber( L, 1 ); \
   luaL_checkvec3( L, v, 2 ); \
   dq_##n( Q.dq, d, v ); \
   lua_pushdq( L, Q ); \
   return 1; }
#define DQL_CR_DVV( n ) \
static int dqL_##n( lua_State *L ) { \
   dqL_t Q; double d, v[3], u[3]; \
   d = luaL_checknumber( L, 1 ); \
   luaL_checkvec3( L, v, 2 ); \
   luaL_checkvec3( L, u, 3 ); \
   dq_##n( Q.dq, d, v, u ); \
   lua_pushdq( L, Q ); \
   return 1; }
#define DQL_CR_M( n ) \
static int dqL_##n( lua_State *L ) { \
   dqL_t Q; double M[3][3]; \
   luaL_checkmat3( L, M, 1 ); \
   dq_##n( Q.dq, M ); \
   lua_pushdq( L, Q ); \
   return 1; }
#define DQL_CR_MV( n ) \
static int dqL_##n( lua_State *L ) { \
   dqL_t Q; double M[3][3], v[3]; \
   luaL_checkmat3( L, M, 1 ); \
   luaL_checkvec3( L, v, 2 ); \
   dq_##n( Q.dq, M, v ); \
   lua_pushdq( L, Q ); \
   return 1; }
#define DQL_CR_Q( n ) \
static int dqL_##n( lua_State *L ) { \
   dqL_t Q; dqL_t *P; \
   P = luaL_checkdq( L, 1 ); \
   dq_##n( Q.dq, P->dq ); \
   lua_pushdq( L, Q ); \
   return 1; }
static int dqL_cr_raw( lua_State *L )
{
   int i;
   dqL_t Q;
   luaL_checktype(L,1,LUA_TTABLE);
   for (i=0; i<8; i++) {
      TBL_GETNUM( L, Q.dq[i], i+1, 1 );
   }
   lua_pushdq( L, Q );
   return 1;
}
DQL_CR_DVV( cr_rotation )
DQL_CR_DVV( cr_rotation_plucker )
DQL_CR_M(   cr_rotation_matrix )
DQL_CR_DV(  cr_translation )
DQL_CR_V(   cr_translation_vector )
DQL_CR_V(   cr_point )
DQL_CR_VV(  cr_line )
DQL_CR_VV(  cr_line_plucker )
DQL_CR_MV(  cr_homo )
DQL_CR_Q(   cr_copy )
DQL_CR_Q(   cr_conj )
DQL_CR_Q(   cr_inv )
/* Operations. */
static int dqL_op_norm2( lua_State *L )
{
   double real, dual;
   dqL_t *Q;
   Q = luaL_checkdq( L, 1 );
   dq_op_norm2( &real, &dual, Q->dq );
   lua_pushnumber( L, real );
   lua_pushnumber( L, dual );
   return 2;
}
#define DQL_OP_Q( n ) \
static int dqL_##n( lua_State *L ) { \
   dqL_t O, *Q; \
   Q = luaL_checkdq( L, 1 ); \
   dq_##n( O.dq, Q->dq ); \
   lua_pushdq( L, O ); \
   return 1; }
#define DQL_OP_QQ( n ) \
static int dqL_##n( lua_State *L ) { \
   dqL_t O, *Q, *P; \
   Q = luaL_checkdq( L, 1 ); \
   P = luaL_checkdq( L, 2 ); \
   dq_##n( O.dq, Q->dq, P->dq ); \
   lua_pushdq( L, O ); \
   return 1; }
DQL_OP_QQ(  op_add )
DQL_OP_QQ(  op_sub )
DQL_OP_QQ(  op_mul )
DQL_OP_Q(   op_sign )
DQL_OP_QQ(  op_f1g )
DQL_OP_QQ(  op_f2g )
DQL_OP_QQ(  op_f3g )
DQL_OP_QQ(  op_f4g )
static int dqL_op_extract( lua_State *L )
{
   dqL_t *Q;
   double R[3][3], d[3];
   Q = luaL_checkdq( L, 1 );
   dq_op_extract( R, d, Q->dq );
   lua_pushmat3( L, R );
   lua_pushvec3( L, d );
   return 2;
}
/* Check stuff. */
static int dqL_ch_get( lua_State *L )
{
   int i;
   dqL_t *Q = luaL_checkdq( L, 1 );
   lua_newtable(L);
   for (i=0; i<8; i++) {
      lua_pushnumber( L, i+1 );
      lua_pushnumber( L, Q->dq[i] );
      lua_settable( L, -3 );
   }
   return 1;
}
static int dqL_ch_unit( lua_State *L )
{
   dqL_t *Q = luaL_checkdq( L, 1 );
   lua_pushboolean( L, dq_ch_unit( Q->dq ) );
   return 1;
}
static int dqL_ch_cmp( lua_State *L )
{
   dqL_t *P, *Q;
   double precision;
   P = luaL_checkdq( L, 1 );
   Q = luaL_checkdq( L, 2 );
   precision = DQ_PRECISION;
   if (lua_gettop(L) > 2)
      precision = luaL_checknumber( L, 3 );
   lua_pushboolean( L, !dq_ch_cmpV( P->dq, Q->dq, precision ) );
   return 1;
}
/* Misc. */
static int dqL_print( lua_State *L )
{
   dqL_t *Q = luaL_checkdq( L, 1 );
   int vert = lua_toboolean( L, 2 );
   if (vert)
      dq_print_vert( Q->dq );
   else
      dq_print( Q->dq );
   return 0;
}



