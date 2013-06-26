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
	    for (int i = 1; i < args.length; i++)
	    {
		TextureGenerator.loadGraph(new FileInputStream(args[i]));
	    }

	    int res = Integer.parseInt(args[0]);

	    for (String n : TextureGenerator.getTextureNames())
	    {
		File file = new File(new File(System.getProperty("user.dir")), n + ".png");
		System.out.println(file.toString());
		BufferedImage img = new BufferedImage(res, res, BufferedImage.TYPE_INT_ARGB);
		img.setRGB(0, 0, res, res, TextureGenerator.generateTexture_ARGB(n, res, res), 0, res);
		if (!ImageIO.write(img, "PNG", file))
		    throw new RuntimeException("Error writing Image");
	    }

	    TextureGenerator.clearCache();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}
    }
}
