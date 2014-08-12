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

#
# This script prepares the calculation and finalizes it, depending on the options.
#

echo_u() {
  echo -e "\033[34;01m>> $1\033[m"
}

echo_s() {
  echo -e "\033[32;01m>> $1\033[m"
  echo
}

echo_e() {
  echo -e "\033[31;01m!! $1\033[m"
}

echo_w() {
  echo -e "\033[33;01m>> $1\033[m"
  echo
}

read_user() {
  echo_u "$1 [$2]"
  read read_user_result
  if [ -z $read_user_result ]; then
    read_user_result=$2
  fi
}

exit_error() {
  echo_e "$1"
  exit 1
}

if [ ! -d output ]; then
  exit_error "Directory 'output' could not be found. Aborting."
fi

cd output

# ToDo: Some trajs seem to bee (0, 0, 0)...

for file in traj_*; do
  rm -rf ids_$file
  mkdir ids_$file
  echo "Parsing $file..."
  cat $file | while read line; do
    if [ "`echo $line |grep ^#;`" == "" ]; then
      echo $line >> ids_$file/$( echo $line| sed 's/^\([0-9]*\),\(.*\)/\1/' )
      echo $line
    fi
  done
done
