This project when built will automatically comvert obj models placed in it into the required SDKMesh format.
See this wiki tutorial for more information aboutr the command line tool be used: 
https://github.com/Microsoft/DirectXTK12/wiki/Rendering-a-model 

1. rename the main model file something like _obj, as obj is a suffix used by VisStudio iteslef and thus may cause issues!
2. Put original files in to the required folder USING FILE EXPLORER, do not just drag and drop here.
2. Right click on Project Add -> Existing Item -> and then select the main obj file
3. Right click on the image now on the project  select Properties
4. Change the Item Type to "Custom Build Tool"
5. Reopen the Properties window for the file and you should now see options available for it
6. Copy over the same values from the exaple model into the boxes for your new file for "All Configurations" "All Platforms"


ADDITIONAL: The model loading system can only cope with a couple of different image formats for loading in the textures on models. 
As set up for the cup._obj this assumes that the textures refered to in the .mtl file can be loaded, hence the -nodds in the command 
line arguements.

Note that .mtl files are usually human readable text and so it is possible to change the images that the material will load just 
by editing the filenames in the file.

If that term is removed the loader will assume that you will convert all the images for the model to dds yourself. You can set up this
project to do that in the same way that the image project does by right clicking on the project. 

Then select Build Dependencies -> Build Customizations.. 

The click the check box for "Image Content Pipeline". 

After that any images added to this project can be set to "Type : Image Content Pipeline" in their properties and will be converted to dds.
