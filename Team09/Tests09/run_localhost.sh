#!/bin/bash

# Team change to your python alias version "python3", "python", "py"
PYTHON_EXECUTABLE="python3"

(cd milestone1 && "$PYTHON_EXECUTABLE" -m http.server 8001) &
(cd milestone2 && "$PYTHON_EXECUTABLE" -m http.server 8002) & 
(cd milestone3 && "$PYTHON_EXECUTABLE" -m http.server 8003) &

wait
