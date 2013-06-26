import javax.imageio.ImageIO;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.awt.image.BufferedImage;
import com.mystictri.neotexture.TextureGenerator;

public class NeoGen
{
    public static void main(String[] args)
    {
	try
	{
	    TextureGenerator.setUseCache(true);
	    for (String arg : args)
	    {
		TextureGenerator.loadGraph(new FileInputStream(arg));
	    }

	    int res = 256;

	    for (String n : TextureGenerator.getTextureNames())
	    {
		System.out.println(n + ".bmp");
		BufferedImage img = new BufferedImage(res, res, BufferedImage.TYPE_INT_ARGB);
		img.setRGB(0, 0, res, res, TextureGenerator.generateTexture_ARGB(n, res, res), 0, res);
		ImageIO.write(img, "BMP", new File(n + ".bmp"));
	    }

	    TextureGenerator.clearCache();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}
    }
}
