This is the readme for the SSIM in CPP.

This SSIM is not as accurate as the MIT and harvard level versions and I am unsure as to why that is.

However, I am still able to detect images that have flipped better than the version of my matlab code.

This does a simple analysis of SSIM that can be found here: https://www.cns.nyu.edu/~lcv/ssim/

A full write up can be found.

The code is fairly simple, I have a mean function, a variance function, a covraiance function and lastly an SSIM which uses all these prior functions
to calculate how similar an image is. Most of the code is just checking if the image is valid. I have used some AI to help as I am not very knowledgeable at CPP. 

The code passes if the images are 85% similar. Otherwise it fails. 