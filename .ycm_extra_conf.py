def Settings(**kwargs):
	return {
		'flags': [
			'-std=c++17',
			'-I./libme/include', '-I./src/util',
			'-DLOCALEDIR=\\"/usr/share/locale\\"', '-DPACKAGE=\\"makeit\\"'
		]
	}
