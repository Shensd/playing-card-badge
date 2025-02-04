
import sys

# i stole this from geeks for geeks, this would probably be a good use for copilot
# https://www.geeksforgeeks.org/run-length-encoding-python/
def runlen_encoding(input):
    encoded = []

        # We start with the first character
    count = 1  
    
    # Loop through the string starting from the second character
    for i in range(1, len(input)):
      
       # If the current character is the same as the previous one
        if input[i] == input[i - 1]: 
            count += 1  # Increment the count
        else:
          
              # Add previous character and its count to output
            # output += f"{input[i - 1]}{count}"  
            encoded.append((input[i - 1], count))
            
            # Reset count for the new character
            count = 1  
    
    # After the loop, we still need to add the last character group
    # output += f"{input[-1]}{count}"
    
    return encoded

def main(argc, argv):
    if argc < 2:
        print(f"usage: {argv[0]} \"-.-- --- ..- .-./... - .-. .. -. --./.... . .-. .\"")
        return

    morse_string = argv[1]

    morse_to_bits = []

    for i in morse_string:
        if i == ".":
            morse_to_bits.append(1)
            morse_to_bits.append(0)
        if i == "-":
            morse_to_bits.append(1)
            morse_to_bits.append(1)
            morse_to_bits.append(1)
            morse_to_bits.append(0)
        if i == " ":
            # space is 3 units but each char has a space at the end of it
            morse_to_bits.append(0)
            morse_to_bits.append(0)
        if i == "/":
            # space between words is 7 units, but each char has a space after it
            morse_to_bits.append(0)
            morse_to_bits.append(0)
            morse_to_bits.append(0)
            morse_to_bits.append(0)
            morse_to_bits.append(0)
            morse_to_bits.append(0)

    encoded = runlen_encoding(morse_to_bits)

    # print(encoded)
    print("uint8_t encoded_morse[] = {")
    # print(f"\t{','.join([(str(x[1])) for x in encoded])}")
    for i, e in enumerate(encoded):
        print(f"{e[1]},", end="")
        if i > 0 and (i + 1) % 16 == 0:
            print()
    print("};")

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)