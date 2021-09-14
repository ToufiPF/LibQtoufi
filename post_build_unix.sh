echo "Copying compiled files from '$2' to '$1'"
mkdir -p $1
cp -f $2/*.so $1
cp -f $2/*.o $1
