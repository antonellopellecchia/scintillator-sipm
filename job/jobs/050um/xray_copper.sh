#!/bin/sh
export HOME="/lustrehome/antonellopellecchia"
source $HOME/sourcefiles/set_garfield.sh
source $HOME/sourcefiles/set_geant4.sh
$HOME/simulations/geant4/xray_copper/build/xray_copper $HOME/simulations/geant4/xray_copper/run.mac out/tree/tree_050.root 50
