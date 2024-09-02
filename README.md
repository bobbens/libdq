### LICENSE

> Copyright 2010-2024 Edgar Simo-Serra
>
> This program is free software: you can redistribute it and/or modify
> it under the terms of the GNU Lesser General Public License as published by
> the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.

> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
> GNU General Public License for more details.

> You should have received a copy of the GNU General Public License
> along with this program. If not, see <http://www.gnu.org/licenses/>.


### OVERVIEW

This is a library for using and manipulating dual quaternions. Dual quaternions are useful for describing rigid body movements using screw theory.

If you use this library please reference it.


### USAGE

To install type:

```sh
$ make install
```

To make documentation (recommended) type:

```sh
$ make docs
```

Docs are located in the `docs/` folder that will be created when you run `make docs`.


### CHANGELOG

 - Version 2.3, October 2021
   - Fixed dq_cr_inv (patch by @thery, thanks!)
 - Version 2.2, February 2013
   - Fixed build on newest Ubuntu
   - Added version command to Lua API
   - Compatibility with Lua 5.2
 - Version 2.1, April 2012
   - Made it more clear license is LGPL
   - Fixed bug in Lua handling of matrices
   - Added defines for getting version
   - Added dq_version, dq_ch_plane_point, dq_cr_plane
   - Minor correctness fixes
 - Version 2.0, April 2011
   - Lua bindings
 - Version 1.5, April 2011
   - Install headers into /usr/include/dq by default instead of prefixing with dq_.
   - Updated documentation
   - Make docs now places final documentation in /
 - Version 1.4, February 2011
   - Fixed major issue in dual quaternion conjugation
   - Added dq_op_extract
   - Added mat3_solve
   - Added vec3_sign
   - Added vec3_distance
   - Minor doxygen improvements
 - Version 1.3, December 2010
   - Cleaned up and documented the auxiliary function files so they can be used.
 - Version 1.2, December 2010
   - Added dq_op_sign to change sign of a dual quaternion.
   - dq_ch_cmp and dq_ch_cmpV now take into account the fact it could be with a different sign.
 - Version 1.1, November 2010
   - Fixed dual quaternion multiplication
   - Implemented more strict unit tests
   - Misc fixes
 - Version 1.0, November 2010
   - Initial Revision


### ACKNOWLEDGEMENTS

A big thanks to Alba Perez for having the patience to deal with my repetitive boring dual quaternion questions and for lending me her notes.
