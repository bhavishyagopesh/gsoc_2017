import sys,io,zipfile
from inmemory_zip_importer import ZipImporter
f = open('common_module_archive.zip','rb')

data = f.read()

f.close()

zipbytes = io.BytesIO(data)

sys.meta_path.append(ZipImporter(zipbytes))
print sys.meta_path
