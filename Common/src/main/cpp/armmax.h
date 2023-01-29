/*      This file is part of Juggluco, an Android app to receive and display         */
/*      glucose values from Freestyle Libre 2 and 3 sensors.                         */
/*                                                                                   */
/*      Copyright (C) 2021 Jaap Korthals Altes <jaapkorthalsaltes@gmail.com>         */
/*                                                                                   */
/*      Juggluco is free software: you can redistribute it and/or modify             */
/*      it under the terms of the GNU General Public License as published            */
/*      by the Free Software Foundation, either version 3 of the License, or         */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      Juggluco is distributed in the hope that it will be useful, but              */
/*      WITHOUT ANY WARRANTY; without even the implied warranty of                   */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                         */
/*      See the GNU General Public License for more details.                         */
/*                                                                                   */
/*      You should have received a copy of the GNU General Public License            */
/*      along with Juggluco. If not, see <https://www.gnu.org/licenses/>.            */
/*                                                                                   */
/*      Fri Jan 27 12:35:09 CET 2023                                                 */


constexpr const int armmax=std::max({
#ifdef __ARM_NR_BASE
 __ARM_NR_BASE,
#endif
#ifdef __ARM_NR_breakpoint
 __ARM_NR_breakpoint,
#endif
#ifdef __ARM_NR_cacheflush
 __ARM_NR_cacheflush,
#endif
#ifdef __ARM_NR_usr26
 __ARM_NR_usr26,
#endif
#ifdef __ARM_NR_usr32
 __ARM_NR_usr32,
#endif
#ifdef __ARM_NR_set_tls
 __ARM_NR_set_tls,
#endif
#ifdef __ARM_NR_get_tls
 __ARM_NR_get_tls,
#endif
0})+1-__ARM_NR_BASE;
