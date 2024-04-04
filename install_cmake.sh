version=3.20
build=1
## don't modify from here
limit=3.20
result=$(echo "$version >= $limit" | bc -l)
os=$([ "$result" == 1 ] && echo "linux" || echo "Linux")
mkdir ./temp
cd ./temp
wget https://cmake.org/files/v$version/cmake-$version.$build-$os-x86_64.sh 
sudo mkdir /opt/cmake
sudo sh cmake-$version.$build-$os-x86_64.sh --prefix=/opt/cmake
