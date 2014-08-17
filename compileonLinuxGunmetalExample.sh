if [[ ! -e bin ]]; then
    mkdir bin
elif [[ ! -d bin ]]; then
    echo "bin already exists but is not a directory" 1>&2
fi

cd bin
cmake .. -DOGDF_PATH:PATH=/home/ivan/projects/medisa_project/ogdf/ -DQT5_CMAKE_PATH:PATH=/home/ivan/programmertools/qt/5.2.1/gcc/
