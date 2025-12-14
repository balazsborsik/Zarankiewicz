import os
import re

# Get the current directory
current_directory = os.getcwd()

# Iterate through all files in the folder
for filename in os.listdir(current_directory):
    
    # Check if file ends with the specific pattern
    if filename.endswith("test1_config.conf"):
        
        # Determine the new filename
        new_filename = filename.replace("test1_config.conf", "test2_config.conf")
        
        print(f"Processing: {filename} -> {new_filename}")
        
        try:
            # Open the original file to read
            with open(filename, 'r') as file_in:
                lines = file_in.readlines()
            
            # Open the new file to write
            with open(new_filename, 'w') as file_out:
                for line in lines:
                    stripped = line.strip()
                    
                    # Check if the line starts with s or t followed by = and a number
                    if re.match(r'^(s|t)(\s*=\s*)(\d+)', stripped):
                        
                        # FIX: Use \g<1>, \g<2>, \g<3> to strictly separate groups from the number 1
                        # This turns "s = 2" into "s = 12"
                        new_line = re.sub(r'^(s|t)(\s*=\s*)(\d+)', r'\g<1>\g<2>1\g<3>', line)
                        file_out.write(new_line)
                    else:
                        file_out.write(line)
                        
            print("--- Success")

        except Exception as e:
            print(f"--- Error processing {filename}: {e}")

print("Batch processing complete.")