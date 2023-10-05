import os
import sys
import random

def create_file(filename, size):
    with open(filename, 'wb') as f:
        f.write(os.urandom(size))

def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <directory> <number_of_files> <max_size_in_kb>")
        sys.exit(1)

    directory = sys.argv[1]
    num_files = int(sys.argv[2])
    max_size_kb = int(sys.argv[3])

    # Convert max size to bytes
    max_size_bytes = max_size_kb * 1024

    # Create directory if it doesn't exist
    if not os.path.exists(directory):
        os.makedirs(directory)

    # Generate files with random sizes
    for i in range(num_files):
        filename = os.path.join(directory, f"file_{i+1}.bin")
        file_size = random.randint(1, max_size_bytes)
        create_file(filename, file_size)
        print(f"Created {filename} with size {file_size} bytes")

if __name__ == "__main__":
    main()