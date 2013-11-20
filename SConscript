env = Environment()
env.ParseConfig('pkg-config --cflags --libs sdl2 glew gl glu')
env.Append(LIBS = 'SDL2_image')
env.Program('GameExe', Glob('./src/*.cpp'))
