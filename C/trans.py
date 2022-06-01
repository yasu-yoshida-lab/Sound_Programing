import os
import sys
import codecs

def trans_file(file_name):
	print(file_name)
	sf = codecs.open(file_name, 'r', encoding='shift-jis')
	uf = codecs.open('utf8_file.txt', 'w', encoding='utf-8')
	for line in sf: uf.write(line)
	sf.close()
	uf.close()
	os.rename('utf8_file.txt', file_name)

def find_file(root_path):
	os.chdir(root_path)
	dirs = [dir_name for dir_name in sorted(os.listdir('./'))]
	for sub_dirs in dirs:
		if (os.path.isdir(sub_dirs)):
			os.chdir(sub_dirs)
			cp_path = os.getcwd()
			files = os.listdir(os.getcwd())
			[trans_file(file_name) for file_name in files if ('.c' in file_name) or ('.h' in file_name)]
		os.chdir('../')

if __name__ == '__main__':
    find_file('./chapter09/')
