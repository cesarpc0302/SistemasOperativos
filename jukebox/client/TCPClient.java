import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

public class TCPClient {

  public final static int SOCKET_PORT = 5000;      // you may change this
  public final static String SERVER = "127.0.0.1";  // localhost

  //public final static int FILE_SIZE = 6922386; // file size temporary hard coded
                                               // should bigger than the file to be downloaded

  public static void main (String [] args ) throws IOException {
    int bytesRead;
    int current = 0;
    FileOutputStream fos = null;
    BufferedOutputStream bos = null;
    Socket sock = null;
    String File_Name = "";
    try {
      sock = new Socket(SERVER, SOCKET_PORT);
      System.out.println("Connecting...");
    BufferedWriter socketWriter = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream()));
    BufferedReader socketReader = new BufferedReader(new InputStreamReader(sock.getInputStream()));
	BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
	String promptMsg = "Please enter a  message: ";
    String outMsg = null;

    System.out.print(promptMsg);
      
      outMsg = consoleReader.readLine();
      socketWriter.write(outMsg);
      socketWriter.flush();
      System.out.println("send file name");
      File_Name = outMsg;

// Read and display the size from the server
      InputStream size = sock.getInputStream();
      //String inMsg = socketReader.readLine();
      byte [] fsize  = new byte [50];
      int f = size.read(fsize,0,50);
      String finalSize = new String(fsize, "UTF-8").trim().replaceAll(" ", "");
      int FILE_SIZE = Integer.parseInt(finalSize);
      System.out.println("Server file size: " + (FILE_SIZE + 1));
      System.out.println(); // Print a blank line
      //System.out.print(promptMsg);
      socketWriter.write("crontrol msg");
      socketWriter.flush();


      // receive file
      System.out.println("file name: " + File_Name);
      byte [] mybytearray  = new byte [FILE_SIZE];
      InputStream is = sock.getInputStream();
      fos = new FileOutputStream(File_Name);
      bos = new BufferedOutputStream(fos);
      bytesRead = is.read(mybytearray,0,mybytearray.length);
      current = bytesRead;

      do {
         bytesRead =
            is.read(mybytearray, current, (mybytearray.length-current));
         if(bytesRead >= 0) current += bytesRead;
      } while(current < FILE_SIZE);

      bos.write(mybytearray, 0 , current);
      bos.flush();
      System.out.println("File " + File_Name
          + " downloaded (" + current + " bytes read)");
    }
    finally {
      if (fos != null) fos.close();
      if (bos != null) bos.close();
      if (sock != null) sock.close();
    }
  }

}
