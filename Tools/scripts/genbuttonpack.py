from PIL import Image, ImageDraw, ImageFont

def create_button_image(text, output_path, size=(200, 80), font_size=30, fillcolor="#007BFF", outlinecolor="#0056b3"):
    # Create a blank image with RGBA mode
    img = Image.new("RGB", size, "#CCCCCC")  # Light gray background
    draw = ImageDraw.Draw(img)
    
    # Draw button shape ( rectangle effect)
    draw.rounded_rectangle([(1, 1), (size[0]-1, size[1]-1)], radius=1, fill=fillcolor, outline=outlinecolor, width=3)
    
    # Load font (ensure a valid font path is provided)
    try:
        font = ImageFont.truetype("arial.ttf", font_size)
    except IOError:
        font = ImageFont.load_default()
    
    bbox = draw.textbbox((0, 0), text, font=font)
    text_width, text_height = bbox[2] - bbox[0], bbox[3] - bbox[1]

    text_x = (size[0] - text_width) / 2
    text_y = (size[1] - text_height) / 2
    
    # Draw text
    draw.text((text_x, text_y), text, font=font, fill="white")
    
    imgr = img #.resize(256,256, Image.Resampling.NEAREST)

    # Save the image
    imgr.save(output_path, "JPEG")
    print(f"Button image saved as {output_path}")

# V1 Specs

# e.g tag=play
# =======
# Folder name = <TAG>.btn
# -------
# Folder Contents auick brown fox

# Unpressed:
# btn_<TAG>_U.alx
# btn_<TAG>_U.tga

# Pressed:
# btn_<TAG>_P.alx
# btn_<TAG>_P.tga

# Hover:
# Up:
# btn_<TAG>_H.alx
# btn_<TAG>_H.tga

# a typical 128x128 tga size is around 65kb

def tagFn(TAG):
    return "btn_"+TAG.replace(' ', '_')

def write_alx(filename, width, height):
    """
    Writes the specified text to the provided filename, adjusting vertex positions
    based on the given width and height.

    :param filename: The name of the file to write to.
    :param width: Full width of the image (64 represents half, so total width is 128).
    :param height: Full height of the image (32 represents half, so total height is 64).
    """
    half_width = width / 2
    half_height = height / 2

    content = f"""//786//
obj[{width}x{height}]{{
v[ {-half_width}, {-half_height}, 0.000000]
n[ 0.000000, 0.000000, 1.000000]
t[ 0.000000, 1.000000]
v[ {half_width}, {-half_height}, 0.000000]
n[ 0.000000, 0.000000, 1.000000]
t[ 1.000000, 1.000000]
v[ {half_width}, {half_height}, 0.000000]
n[ 0.000000, 0.000000, 1.000000]
t[ 1.000000, 0.000000]
v[ {-half_width}, {-half_height}, 0.000000]
n[ 0.000000, 0.000000, 1.000000]
t[ 0.000000, 1.000000]
v[ {half_width}, {half_height}, 0.000000]
n[ 0.000000, 0.000000, 1.000000]
t[ 1.000000, 0.000000]
v[ {-half_width}, {half_height}, 0.000000]
n[ 0.000000, 0.000000, 1.000000]
t[ 0.000000, 0.000000]
}}
vertex_count[6]
//Good Luck from Tools\scripts\genbuttonpack.py !!
"""

    try:
        with open(filename, "w") as file:
            file.write(content)
        print(f"File '{filename}' written successfully with width={width} and height={height}.")
    except Exception as e:
        print(f"Error writing to file '{filename}': {e}")

outputdir = ""

def generate_set(TAG):
    _TAG = tagFn(TAG)
    global outputdir
    create_button_image(TAG, outputdir+"/"+ _TAG + "_U.jpg",fillcolor="#007BFF")
    write_alx(outputdir+"/"+ _TAG + "_U.alx", 128,64)
    create_button_image(TAG, outputdir+"/"+ _TAG + "_P.jpg",fillcolor="#AA7BFF")
    write_alx(outputdir+"/"+ _TAG  + "_P.alx", 128,64)
    create_button_image(TAG, outputdir+"/"+ _TAG + "_H.jpg",fillcolor="#00AAFF")
    write_alx(outputdir+"/"+ _TAG  + "_H.alx", 128,64)

def generate_alx():
    pass

import os
def create_directory(path):
    try:
        os.makedirs(path, exist_ok=True)
        print(f"Directory '{path}' created successfully.")
    except Exception as e:
        print(f"Error creating directory '{path}': {e}")

tag = input("Enter TAG (button text)=")
outputdir = (tag.replace(' ','_'))+".btn"
create_directory(outputdir)
generate_set(tag)
generate_alx()
