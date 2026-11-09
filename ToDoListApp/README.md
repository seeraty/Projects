# To Do-list Application
## Description
*Project assignment for course DD1349 by Seerat Yasir and Ava Vatanchi.*

A simple command-line To-Do list application that allows users to add, edit, remove, view and navigate through tasks for specific dates, weeks, and months. The program is interactive and feature an opening message when running the program which shows the current week's To Do-list. The idea is to store the tasks in a file and read/edit from it. 

### Usage

1. Run the ToDoList.java in chosen command-line.
2. The program will welcome you and show you current week's tasks if they exist. It will then provide prompts for adding, editing, removing, and viewing tasks. Choose one of the options, or type `help` to get a documentation of the options.
3. Follow the prompts to interact with the To-Do list application.
4. Type `exit` to leave the program.

### Available commands

`View` - Displays a To-Do List from a time period (date, week or month) and lets user navigate through it to previous or next time period. Option `All` displays an overview of all your stored tasks. Exit the viewing mode by typing `exit`.
 
`Add` - Lets user create a task by inputting a title/description of your task and its due date.
  
`Edit` - Changes a task's name and/or date. Option same exists which lets the user keep the same task name or date. 

`Remove` - Removes the task specified by inputting the task's name and date. This action is non-undoable.

`Help` - Displays documentation about all commands and options.

`Exit` - Exits the program (or viewing mode if `exit` is typed there).

### Libraries

The application uses Java's built-in LocalDate, DateTimeFormatter, and other date and time-related classes for date manipulation and formatting. All libraries are Java's built-in ones.

## Note
For more information about the libraries, or to understand the code, read [doc.md](https://gits-15.sys.kth.se/seeraty/Projektuppgift-DD1349/blob/main/doc.md).
