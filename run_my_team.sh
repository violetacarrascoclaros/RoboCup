#!/bin/bash

# Pide de entrada el nombre del equipo
echo "Ingrese el nombre del equipo:"
read TEAM_NAME

# Definir la ubicación del ejecutable player (se encuentra en la carpeta build)
PLAYER_EXECUTABLE="./build/player"

# Definir el nombre de tu equipo y los puertos base
BASE_PORT=6200

# Ejecutar jugadores de tu equipo
for i in {0..10}; do
    PORT=$(($BASE_PORT + $i))
    xterm -e /bin/bash -l -c "$PLAYER_EXECUTABLE $TEAM_NAME $PORT" &
    sleep 1
done
