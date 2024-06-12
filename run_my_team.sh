#!/bin/bash

# Pide de entrada el nombre del equipo
echo "Ingrese el nombre del equipo:"
read TEAM_NAME

# Definir la ubicación del ejecutable player (se encuentra en la carpeta build)
PLAYER_EXECUTABLE="./build/player"

# Definir el nombre de tu equipo y los puertos base
BASE_PORT=8000

# Ejecutar jugadores de tu equipo
for i in {0..10}; do
    PORT=$(($BASE_PORT + $i))
    $PLAYER_EXECUTABLE $TEAM_NAME $PORT &
done

# Esperar a que todos los procesos finalicen
wait
