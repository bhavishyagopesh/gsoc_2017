# An zip-archive for common-modules imported at startup

import sys
import zipfile

if __name__=='__main__':
   _zf = zipfile.PyZipFile('common_module_archive.zip', 'w')
   try:
       _zf.writepy('/usr/lib/python3.5/_weakrefset.py')
       _zf.writepy('/usr/lib/python3.5/abc.py')
       _zf.writepy('/usr/lib/python3.5/encodings/')
       _zf.writepy('/usr/lib/python3.5/codecs.py')
       _zf.writepy('/usr/lib/python3.5/io.py')
       _zf.writepy('/usr/lib/python3.5/os.py')
   
   finally:
       _zf.close()
   sys.path.insert(0, 'common_module_archive.zip')





    
    
    
    

      
      
    


