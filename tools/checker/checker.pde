PImage img;
String[] coords;
int startingLine = 0;
ArrayList<Col> col;

void setup() {
  size(1024, 1024, P2D);
  col = new ArrayList<Col>();
  
  coords = loadStrings("points.asc");
  
  for (int i=0; i<coords.length; i++) {
    if (!coords[i].startsWith("0, 0, 0")) {
      startingLine = i;
      println("Starting line is: " + startingLine);
      break;
    }
  }
  for (int i=startingLine; i<coords.length; i++) {
    String[] coord = coords[i].split(",");
    col.add(new Col(float(coord[0]), float(coord[1]), float(coord[2])));
  }
  
  img = createImage(256, 256, RGB);
  
  img.loadPixels();
  for (int i=0; i<img.pixels.length; i++) {
    img.pixels[i] = col.get(i).c;
  }
  img.updatePixels();
}

void draw() {
  image(img, 0, 0, width, height);
}

class Col {
  
  color c;
  
  Col(float x,  float y, float z) {
    int r = int(255 * x);
    int g = int(255 * y);
    int b = int(255 * z);
    c = color(r,g,b);
    println(r + ", " + g + ", " + b);
  }
  
}
