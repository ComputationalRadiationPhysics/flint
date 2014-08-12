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
# This file parses setup.gdml for GPT and starts the application
#


if [ ! -h setup/setup.gdml ]; then
  ln -s setup_standard.gdml setup/setup.gdml
fi


found=0
rm -f config/gpt_conf.in
cat setup/setup.gdml | while read line; do
  echo $line |grep "<gpt>" >/dev/null
  if [ $? == "0" ]; then
    found=1
  else
    if [ $found == "1" ]; then
      echo $line |grep "</gpt>" >/dev/null
      if [ $? == "0" ]; then
        found=0
      else
        echo $line >> config/gpt_conf.in
      fi
    fi
  fi
done

GPTDIR=`cat config/gptdir`

./build.sh --compile --verbose && bin/flint -o result.gdf config/gpt_conf.in GPTLICENSE=1277475239

# rm -f result.gdf

echo "Finished."
