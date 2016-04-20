class HelloThread extends Thread
{
	public void run()
	{
		System.out.println("The thread is running");
	}

}

public class ThreadCreation
{
	public static void main(String args[])
	{
		new HelloThread().start();
	}
}