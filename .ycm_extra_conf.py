def Settings(**kwargs):
	return {
		'flags': [ '-std=c++17', '-lme', '-Lexternal/melib', '-Iexternal/melib/include', '-Iexternal/toml11' ],
	}
