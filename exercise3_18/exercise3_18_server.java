import java.net.*;
import java.io.*;

public class exercise3_18_server
{
  public static void main(String[] args)
  {
    System.out.println("Hello! It is server.");
    int length = 0;
    byte[] data = new byte[1024];
    byte[] message = new byte[1024];
    String readData = "";
    String outputData = "";
    try
    {
      ServerSocket sock = new ServerSocket(17);
      while (true)
      {
        Socket client = sock.accept();
        InputStream in = client.getInputStream();
        ByteArrayOutputStream buffer = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(client.getOutputStream());

        length = in.read(data);
        while (length != -1)
        {
          //print read data
          readData = new String(data, "UTF-8");
          System.out.println("Get : " + readData);

          //read data to buffer
          buffer.write(data, 0, length);
          buffer.flush();

          //output data that type is byte array
          message = buffer.toByteArray();
          out.write(message);
          out.flush();
          buffer.reset();

          //print output data
          outputData = new String(message, "UTF-8");
          System.out.println("Return : " + outputData);

          length = in.read(data);
        }
        client.close();
      }
    }
    catch (IOException ioe)
    {
      System.err.println(ioe);
    }
  }
}
