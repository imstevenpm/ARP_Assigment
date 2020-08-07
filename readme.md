# Instructions for use

The following instructions were tested using Ubuntu Focal Fossa 20.04.1 LTS on August 6th, 2020. For a more recent guide, check the Readme.md file from the repository online.


    1. Open a new terminal in a chose directory and clone the repository given in \cite{b3} by running:
    `
    git clone https://github.com/imstevenpm/ARP_Assigment.git`
    2. In the same shell, execute the bash script by running: `
    ./Bash_StevenPalma.bash\end` 
    A new terminal will show up and the SnPnLn and Gn executables will be created in the current directory.
    3. Before continuing, the user can set the parameters from the Config\_StevenPalma.config file to a different ones. In the new terminal, execute the SnPnLn executable by running:
    `
    ./SnPnLn` 
    The network should initialize by showing on shell the PID of each Posix process created. Also, a time delay of 10 seconds is waited before Gn sends the first message to Pn and the communication cycle starts.
    4. After the 10 seconds, messages from all the Posix processes will appear in the shell indicating their current status. The network is up and running.
    5. Notice that the Log\_StevenPalma.log file is created as well as the FIFO files for the nammed pipes in the current directory of the user's machine.
    6. From here, -in a different shell- the user can send the console signals to Sn for displaying the Log\_StevenPalma.log content by running:
    '
    kill -SIGUSR2 <Sn PID>'
    or for starting/stopping Pn from receiving messages by running:
    '
    kill -SIGUSR1 <Sn PID>'
    7. To end the execution of the network the user can type CTRL+C


# Additional Observations
    - For Pn (1), the requirement is marked as yellow because the application of the network defined in \cite{b1} and \cite{b2} of computing the token as a sinusoidal wave wasn't accomplished because the formula didn't work properly. However; using the new formula proposed by the designer, the system works fine.
    - For Gn (1), the network was designed originally for the requirements of \cite{b1} so It isn't well optimized for \cite{b2}. This results in Gn sometimes not communicating properly with Pn, however a simply restart of the network solves it.
    - For Sn (2), the signals can be sent and the processes will do what required; however, after a console signal was sent, the communication cycle stops since Pn listens first in the select to messages coming from Sn. This causes that, after a console message is sent, Pn doesn't receive anymore messages from Gn since Gn is still waiting for the reply from the socket that Pn should have sent when Gn sent the message, but Pn won't do it because it listened to the named pipe from Sn and not the one from Gn. Since Gn is waiting the reply, It won't send any message and Pn won't receive any neither. This didn't happen when the network was used for the application described in \cite{b1} since Gn and Gn-1 were different and independent. 
    In order to solve this, each time a console signal is sent and the communication cycle is wanted to be restarted again, then the user first must kill the current Gn process running:
    `
    kill -9 <Gn PID>`
    And then execute a new Gn process by running:
    `
    ./Gn`
    
    - For Config file (1), the parameters are well read and used in the network; however, the IP address wasn't used since there wasn't any other additional machine to be tested on. This means that even if the network should work between different machines, It wasn't tested. Also, the designer noticed that the network is sensitive to the time delay for the Pn process specified in the Config file. This value should be well tuned accordingly to the timeouts of the selects used and the initial delays of the network. This didn't happen when the network was tested for \cite{b1}, since this time delay was an add-on from \cite{b2}.
    - Regarding the files created after the bash file and the processes are executed, it is advisable for the user to delete all the files generated each time a new network is about to be started (Log file, executables and pipes). since for example, the Log file is never emptied, so It might have values from past experiments. This was done like this because no requirement was given about what to do with the log file when the network finishes.
    - Regarding the messages displayed in the shell when the network is running, sometimes the messages get overlapped. This happens because each process is executing simultaneously so It is expected that their print functions to get overlapped in the shell. However; this is something that only happens in the shell and It doesn't happen in the Log file where the event are registered without overlapping.
