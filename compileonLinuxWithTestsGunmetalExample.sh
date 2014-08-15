if [[ ! -e bin ]]; then
    mkdir bin
elif [[ ! -d bin ]]; then
    echo "bin already exists but is not a directory" 1>&2
fi

cd bin
cmake ..  -DBUILD_TESTING=ON -DOGDF_PATH:PATH=/home/ivan/projects/medisa_project/ogdf/ -DQT5_CMAKE_PATH:PATH=/home/ivan/programmertools/qt/5.2.1/gcc/ -DCMAKE_BUILD_TYPE=Debug -DGMOCK_ROOT:PATH=/home/ivan/projects/medisa_project/gmock-1.7.0/bin/ -DGMOCK_INCLUDE_DIR:PATH=/home/ivan/projects/medisa_project/gmock-1.7.0/include/ -DGTEST_ROOT:PATH=/home/ivan/projects/medisa_project/gmock-1.7.0/bin/gtest/ -DGTEST_INCLUDE_DIR:PATH=/home/ivan/projects/medisa_project/gmock-1.7.0/gtest/include/
