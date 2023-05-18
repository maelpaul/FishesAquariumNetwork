[1mdiff --git a/AfficheurPoisson/src/ProjetPoisson/project/scenes/MenuScene.java b/AfficheurPoisson/src/ProjetPoisson/project/scenes/MenuScene.java[m
[1mindex 2459d7f..b3d1d63 100644[m
[1m--- a/AfficheurPoisson/src/ProjetPoisson/project/scenes/MenuScene.java[m
[1m+++ b/AfficheurPoisson/src/ProjetPoisson/project/scenes/MenuScene.java[m
[36m@@ -39,7 +39,9 @@[m [mpublic class MenuScene extends Scene {[m
     }[m
 [m
     public final static int PING_TIME = 35; // 40 in reality[m
[31m-    public final static int ATTEMPT_CONNECTION_TIME = 2; // 40 in reality[m
[32m+[m[32m    public final static int ATTEMPT_CONNECTION_TIME = 2;[m
[32m+[m
[32m+[m[32m    public final static int WAIT_FOR_EXIT = 5;[m
 [m
     private Timer pingTimer;[m
     private Timer attemptConnexionTimer;[m
[1mdiff --git a/AfficheurPoisson/src/ProjetPoisson/project/threads/ClientThread.java b/AfficheurPoisson/src/ProjetPoisson/project/threads/ClientThread.java[m
[1mindex aa554c8..4bbbab2 100644[m
[1m--- a/AfficheurPoisson/src/ProjetPoisson/project/threads/ClientThread.java[m
[1m+++ b/AfficheurPoisson/src/ProjetPoisson/project/threads/ClientThread.java[m
[36m@@ -42,7 +42,6 @@[m [mpublic class ClientThread extends CommunicationThread {[m
         try {[m
             while (this.running){[m
                 Thread.sleep(500); // Add a delay of 500 milliseconds[m
[31m-                //System.out.println("loop");[m
 [m
                 if (client.isConnected()){[m
                     if (messageToSend.size() > 0 && messageReceived == messageProcessed) {[m
[36m@@ -51,13 +50,18 @@[m [mpublic class ClientThread extends CommunicationThread {[m
                         client.sendMessage(toSend);[m
 [m
                         messageProcessed += 1;[m
[31m-                    } else if (messageReceived < messageProcessed) {[m
[31m-                        String result = client.readMessage();[m
[31m-                        if (result != null){[m
[31m-                            System.out.println("Messaged received(" + messageReceived + ") :" + result);[m
[31m-                            receivedMessages.add(new Message(messageReceived, result));[m
[31m-                            messageToSend.remove(messageReceived);[m
[31m-                            System.out.println(didReceiveMessage());[m
[32m+[m[32m                    }[m
[32m+[m
[32m+[m[32m                    String result = client.readMessage();[m
[32m+[m[32m                    if (result != null) {[m
[32m+[m[32m                        System.out.println("Messaged received(" + messageReceived + ") :" + result);[m
[32m+[m[32m                        receivedMessages.add(new Message(messageReceived, result));[m
[32m+[m[32m                        messageToSend.remove(messageReceived);[m
[32m+[m
[32m+[m[32m                        if (result.contains("list")) {[m
[32m+[m[32m                            if (messageReceived != messageProcessed)[m
[32m+[m[32m                                messageReceived += 1;[m
[32m+[m[32m                        } else {[m
                             messageReceived += 1;[m
                         }[m
                     }[m
