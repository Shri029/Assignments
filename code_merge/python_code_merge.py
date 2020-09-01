import os
import platform
from pathlib import Path
import pandas as pd
from datetime import date
from multiprocessing import Pool
import re
import pandas as pd
import time


def validate_folders(start_date, end_date, folders, input_path):
    """
     input parameters: start date, end date, list of folders, path of folder
     processing : filter out the folder existing between start_date and end_date
     returns: list of folders between start_date and end_date
    """
    valid_paths_to_files = []
    for folder in folders:
        #converting folder name into date
        folder_date = date(int(folder[0:4]), int(folder[4:6]), int(folder[6:8]))
    
        if ((end_date - start_date).days) - ((folder_date - start_date).days)>= 0 :
            valid_path = str(input_path) + '\\' + str(folder)
            valid_paths_to_files.append(valid_path)
    return valid_paths_to_files        


def file_to_df(path_to_file):
   """
       input parameters: path to a csv file
       processing : inserting new columns into dataframe according to name of the file and further rearranging columns
       returns: processesed dataframe
   """   
   dataframe = pd.read_csv(path_to_file)
   file_name = os.path.splitext(os.path.basename(path_to_file))[0]
   file_name = file_name.split()

   
   #inserting symbol, expiry, option either call or put, and strike price of contract
   
   dataframe["Symbol"] = file_name[0]
   dataframe["ExpiryDate"] = file_name[1] + file_name[2] + file_name[3]
   dataframe["OptionType"] = file_name[4]
   dataframe["StrikePrice"] = file_name[5]

   cols = list(dataframe)

   cols.insert(0, cols.pop(cols.index('Symbol')))
   cols.insert(1, cols.pop(cols.index('ExpiryDate')))
   cols.insert(2, cols.pop(cols.index('OptionType')))
   cols.insert(3, cols.pop(cols.index('StrikePrice')))

   #rearranging dataframe
   dataframe = dataframe.loc[:, cols]
   return dataframe

def merge_files(start_date, end_date):
    """
     input parameters: start_date, end_date
     processing : filters out folder not in the range of given dates, filters out csv file, merges them using
     multiprocessing(pool), sorts output dataframe accordingg to timestamp, and writes csv file in output folder
    """ 
    input_path = Path('D:\Code_Merge_Test\\SampleInputDirectory')
    mid_path = Path('D:\\Code_Merge_Test')

    #folders in given path
    folders = os.listdir(input_path)

    #filtering folder
    valid_paths_to_files = validate_folders(start_date, end_date, folders, input_path)
    files = os.listdir(valid_paths_to_files[0])

    #merging files of each folder
    for folder in valid_paths_to_files:
        # starting time to process folder       
        start_time = time.time()
        
        #print("Folder:", folder)
        files = os.listdir(folder)
        csv_file_paths = []
        
        for filename in files:
            #picking csv file 
            if filename.endswith(".csv"):
             path_to_file = str(folder) + '\\'+ str(filename)
             #list withpath of csv file
             csv_file_paths.append(path_to_file)
             #print("csv file: ",filename)
        print()
        #merging gile using parallel processing
        pool = Pool(processes=3)

        #file_to_df - name of function, csv_file_paths - containg path of csv file(function argument)
        res = pool.map(file_to_df, csv_file_paths)
        combined_df = pd.concat(res, ignore_index=True)
        
        #sorting file using timesatmp column
        combined_df.sort_values(by='Timestamp')
        print(combined_df)

        #writing output
        folder_name = os.path.basename(folder)
        folder_path = str(mid_path) + '\OutputDirectory\\' + str(folder_name) + '.csv'
        
        combined_df.to_csv(folder_path)
        print(folder_path)

        # end time
        print("Run time: ", time.time() - start_time )


# __name__ 
if __name__=="__main__":
    start_date = date(2019, 11, 1)
    end_date = date(2019, 11, 4)
    merge_files(start_date, end_date) 





