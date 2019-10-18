import os
env=Environment()

env.Append( CPPFLAGS='/EHsc /MD' )
env.Append( CPPPATH=['C:\\third_party\\boost_1_69_0\\boost_1_69_0'] )
env.Append( LIBPATH=['C:\\third_party\\boost_1_69_0\\boost_1_69_0\\stage\\lib'] )
env.Append( LIBS=['libboost_date_time-vc141-mt-x64-1_69'] )

#if ARGUMENTS.get('debug', 0):
#    env.Append(CPPDEFINES = ['DEBUG', '_DEBUG'])
#    env.Append(CCFLAGS='/MDd')
#    env.Append(CCFLAGS=Split('/Zi /Fd${TARGET}.pdb'))
#    env.Append(LINKFLAGS = ['/DEBUG'])
#    variant = 'Debug'
#else:
#    env.Append(CPPDEFINES = ['NDEBUG'])
#    variant = 'Release'

env.Program(target = 'helloworld', source = ["main.cxx"])