import java.net.*;
import java.io.*;

public class exercise3_16_server
{
  public static void main(String[] args)
  {
    System.out.println("Hello! It is server.");
    try
    {
      ServerSocket sock = new ServerSocket(17);

      /* now listen for connections */
      while (true)
      {
        Socket client = sock.accept();
        PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
        //client.getOutputStream() is output stream
        //the true means that the println, printf, or format methods will flush the output buffer

        /* write the Date to the socket */
        pout.println("Never Give Up!");

        /* close the socket and resume */
        /* listening for connections */
        client.close();
      }
    }
    catch (IOException ioe)
    {
      System.err.println(ioe);
    }
  }
}
