#!/bin/bash

# Copyright 2009 Lucas Clemente
#
# This file is part of FLINT.
#
# FLINT is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FLINT is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FLINT.
# If not, see <http://www.gnu.org/licenses/>.
#

echo $1

cat $1 |grep -v ID |grep -v Traj |sed -e 's/ .*$//g' -e '/^[ \t]*$/d' |sort |uniq > tmp_ids

if [ $2 == "" ]; then
  2=output
fi

rm -f $2

cat tmp_ids | while read id; do
  echo $id
  echo "ID $id" >> $2
  echo "  ID2 x y z Bx By Bz E" >> $2
  grep "^$id " $1 | while read line; do
    echo "  $line" >> $2
  done
  
  echo >> $2
done

rm -f tmp_ids

exit 0
