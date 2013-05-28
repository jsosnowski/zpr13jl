env = Environment()

#       Add header search path
env.Append(CPPPATH = ['/usr/include', '/usr/local/include'])

#       Add compile-time flags
env.Append(CCFLAGS=['-Wall','-g', '-pedantic'])

# libraries to link against
# Notice you dont need the '-l', since SCons is platform independent
env.Append(LIBS=[
  'wt', 'wthttp', 'pthread',
  'boost_random', 'boost_regex', 'boost_signals',
  'boost_system', 'boost_thread', 'boost_filesystem',
  'boost_program_options', 'boost_date_time'
])

env.Append(CXXFLAGS="-std=c++11")

#       Add library search path
env.Append(LIBPATH = ['/usr/lib','/usr/local/lib', '/opt/lib'])

# Compile and link the binary
env.Program('game',['mainApp.cpp', 'GameWidget.h','GameWidget.cpp',
'GameServer.h', 'GameServer.cpp', 'GEvent.h', 'GEvent.cpp', 'BoardWidget.h', 'BoardWidget.cpp',
'Field.h', 'Field.cpp'])
