def Settings(**kwargs):
	return {
		'flags': [
			'-I./libme/include', '-I./src/util',
			'-DLOCALEDIR=\"/usr/share/locale\"', '-DPACKAGE=\"makeit\"'
		]
	}
