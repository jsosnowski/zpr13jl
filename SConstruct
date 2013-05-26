env = Environment()

#       Add header search path
env.Append(CPPPATH = ['/usr/include', '/usr/local/include'])

#       Add compile-time flags
#env.Append(CCFLAGS=['-Wall','-g'])

# libraries to link against
# Notice you dont need the '-l', since SCons is platform independent
env.Append(LIBS=[
  'wt', 'wthttp',
  'boost_random', 'boost_regex', 'boost_signals',
  'boost_system', 'boost_thread', 'boost_filesystem',
  'boost_program_options', 'boost_date_time'
])

#       Add library search path
env.Append(LIBPATH = ['/usr/lib','/usr/local/lib', '/opt/lib'])

# Compile and link the binary
env.Program('test',['GameApp.cpp' , 'GameServer.h', 'GameServer.cpp', 'GameWidget.h', 'GameWidget.cpp', 'GEvent.h', 'GEvent.cpp'])
