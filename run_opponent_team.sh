#!/bin/bash

# Pide de entrada el nombre del equipo rival
echo "Ingrese el nombre del equipo rival:"
read OPPONENT_TEAM_NAME

# Definir la ubicación del ejecutable player (se encuentra en la carpeta build)
PLAYER_EXECUTABLE="./build/player"

# Definir puertos base

OPPONENT_BASE_PORT=8012

# Ejecutar jugadores del equipo rival
for i in {0..10}; do
    PORT=$(($OPPONENT_BASE_PORT + $i))
    $PLAYER_EXECUTABLE $OPPONENT_TEAM_NAME $PORT &
done

# Esperar a que todos los procesos finalicen
wait
