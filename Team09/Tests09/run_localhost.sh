#!/bin/bash

# Team change to your python alias version "python3", "python", "py"
PYTHON_EXECUTABLE="python"

(cd  && "$PYTHON_EXECUTABLE" -m http.server 8001) & cd ..
(cd  && "$PYTHON_EXECUTABLE" -m http.server 8002) & cd ..
(cd  && "$PYTHON_EXECUTABLE" -m http.server 8003) & cd ..

wait
