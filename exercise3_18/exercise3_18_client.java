import java.net.*;
import java.io.*;
import java.util.*;

public class exercise3_18_client
{
  public static void main(String[] args)
  {
    Scanner scanner = new Scanner(System.in);
    int length = 0;
    String message = "";
    String returnData = "";
    byte[] byteOfMessage = new byte[1024];
    byte[] data = new byte[1024];
    try
    {
      Socket sock = new Socket("127.0.0.1", 17);
      InputStream in = sock.getInputStream();
      ByteArrayOutputStream buffer = new ByteArrayOutputStream();
      DataOutputStream out = new DataOutputStream(sock.getOutputStream());

      message = scanner.nextLine();
      while (!message.equals("exit"))
      {
        //output message
        byteOfMessage = message.getBytes();
        out.write(byteOfMessage);

        //read data that is server return
        length = in.read(data);
        if (length != -1)
        {
          //read data to buffer
          buffer.write(data, 0, length);
          buffer.flush();

          //print return data
          returnData = new String(buffer.toByteArray(), "UTF-8");
          System.out.println("ServerReturn : " + returnData);

          buffer.reset();
        }
        message = scanner.nextLine();
      }
      sock.close();
    }
    catch (IOException ioe)
    {
      System.err.println(ioe);
    }
  }
}
