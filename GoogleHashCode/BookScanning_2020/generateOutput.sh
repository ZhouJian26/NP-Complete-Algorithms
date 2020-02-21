echo "Preparing Working Space"
rm -rf ./Output
mkdir -p Output

echo "Compile file"
g++ main.cpp -o ./Output/main -std=c++17 -Wall -O3    

echo "Copy source code to Output folder"
cp ./main.cpp ./Output/ 

for entry in "Input"/*
do
s=${entry%.*}
s=${s#*/}
echo "Running command: cat $entry | ./Output/main > Output/$s.out.txt"
cat $entry | "./Output/main" > "Output/$s.out.txt"
done

echo "Deleting compiled file"
rm ./Output/main