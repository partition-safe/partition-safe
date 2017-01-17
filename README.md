PartitionSafe
=============

The *f*ault of the century (but don't actually use it)
This is a school project, so use at your own risk.

Project structure
-----------------

We have separated this project into 2 components; our library "lib/libpartitionsafe" and our GUI (a QT project) "src".
Our library is responsible for all communication with our vault and key store also for creating these.
The GUI on the other hand communicates only with the end user and our library. 

### Library

Our library consists of 3 main classes; PartitionSafe, Vault and Partition.

 - **PartitionSafe** With this class you can create new vaults and it holds everything together.
 - **Vault** Manages our vault file, header, its initial size and such
 - **Partition** Responsible for all our file operations


### GUI

Our GUI program consists of a main window and various dialogs to interact with the users.


License
-------

This project is licensed under MIT.