from tkinter import * 
from PIL import ImageTk,Image
from ctypes import cdll 




  
# load the library 
#lib = cdll.LoadLibrary('../Histo_Library.o') 
  
# create a Geek class 
#class Histo(object): 
  
    # constructor 
#    def __init__(self): 
  
        # attribute 
#        self.obj = lib.Histo() 
  
    # define method 
#    def myFunction(self): 
#        lib.ReadTXT(self.obj) 
  
# create a Geek class object 
#f = Geek() 















# This needs to be clean... Likely need to put everything into a class and then use a main script to call from class
window = Tk() 

def PlotData():
	window.quit()

def ReadData():
	window.quit()



def main():

	# Initiate Tk window
#	window = Tk() 
	window.title("Histomaster")

	# Get the icon file from Images
	window.iconbitmap(r"../Images/Histomaster_colour.ico")

	# Add text
	frame = LabelFrame(window, text="Histomaster will plot your data without the hassle of using the ROOT directory!", padx=5, pady=5)
	frame.grid(row=0, column=0, padx=10, pady=10)

#	img = Image.open(r"../Images/HistomasterLogo_2.png")
#	img = img.resize((450, 450), Image.ANTIALIAS) # The (250, 250) is (height, width) 
#	img.save("Logo.png", "png") # The only reason I included this was to convert
	Logo = ImageTk.PhotoImage(file="Logo.png")
	label = Label(image=Logo)
	label.grid(row=1, column=0, columnspan=3)

	status = Label(window, text="Plot progress: n%", bd=2, relief=SUNKEN, anchor=E) #+ str(len()))
	status.grid(row=3, column=0, columnspan=3, sticky=W+E)


	button_input = Button(frame, text="Input data file", command=ReadData)
	button_input.grid(row=2, column=1, pady=20)

	button_plot = Button(frame, text="Plot data", command=PlotData)
	button_plot.grid(row=2, column=2, padx=15, pady=20)

	window.mainloop()



main()

