cd "$MESON_BUILD_ROOT"
fusermount -u x
rm -f blargh
echo "##########################"
echo "Setting up filesystem..."
./mkfs.tfs ./blargh
mkdir x
clear
echo "##########################"
echo "Mounting filesystem..."
./mount.tfs ./x

cd x

echo "Hello World1" > TestFile1
ls -l Testfile1; cat TestFile1
echo "Hello World2" >> TestFile1
cp TestFile1 TestFile2
ls -l TestFile2; cat TestFile2
cp TestFile1 TestDir1/TestFile3
ls -l TestDir1/TestFile3; cat TestDir1/TestFile3
cd TestDir2; echo "Test4" > TestFile4
ls -l testFile4; cat TestFile4
use an editor to create TestFile4
cp TestFile4  ../TestDir1/TestDir3/TestFile5
ls -l ../TestDir1/TestDir3/TestFile5; cat ../TestDir1/TestDir3TestFile5
rm TestFile4
cd ..; rmdir TestDir1
rmdir TestDir2
du -s TestFile2
