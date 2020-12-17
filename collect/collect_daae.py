#!/usr/bin/python3

import os
import glob
import sys
import csv
import enum
import re
import shutil

warehouse_path = "../warehouse/"
output_path = "./output/"

class WriteType(enum.Enum):
	NONE = 0
	CLASS = 1
	CLASS_MEM = 2
	ENUM = 3
	ENUM_MEM = 4
	RECEIVER_NAME = 5
	RECEIVER_FUNC = 6

class DuplicateError(Exception):
	def __init__(self):
		super().__init__("It has duplicate value. Please check and try again.")

#get the latest file name
def getLatestFileName():
	csv_dir = warehouse_path + "*.csv"
	list_of_files = glob.glob(csv_dir)
	if not list_of_files:
		print("!!!Error (getLatestFileName) : There is no file")
		sys.exit()
	latest_file = max(list_of_files, key=os.path.getctime)
	_, fileName = os.path.split(latest_file)
	return fileName

#get the build service name
def getBuildServiceName(fileName):
	splited_filename = fileName.split('-')
	build_service_name = splited_filename[7]
	print(">> The build service name is : " + build_service_name)
	return build_service_name

#get the service name
def getServiceName(row):
	splited_servicename = row.split(' ')
	service_name = splited_servicename[3]
	print(">> The service name is : " + service_name)
	return service_name

#get services from the latest file
def getServices(fileName, services):
	filepath = warehouse_path + fileName
	print(">> Get services from : " + fileName)
	with open(filepath, 'r', encoding='cp1252') as csvfile:
		rdr = csv.reader(csvfile)
		for row in rdr:
			try:
				if row[0] == '[HEADER]xxEnum':
					services.append('tidl')
				elif row[0].startswith('[HEADER]xxEnum_'):
					service_temp = row[0].replace('[HEADER]xxEnum_', '')
					services.append(service_temp)
			except Exception as e:
				print("!!!Error (getServices): ", e)
				exit()

	print("-----------------------------------------------------------------")
	print(services)
	print("-----------------------------------------------------------------")

#check the line has valid value or not
def checkValidValue(line):
	result = True
	joined_line = [v for v in line if v]
	#if it is empty, it is invalid
	if len(joined_line) == 0:
		result = False
	#if it is comments, it is invalid
	if line[0].startswith('#') == True:
		result = False
	return result

#make csv file for ouptut
def writeOutput(lines, file_name):
	print(">> Start to write output : " + file_name)
	output_filename = output_path + file_name
	f = open(output_filename, "w", encoding='cp1252')
	writer = csv.writer(f)
	writer.writerows(lines)
	f.close()
	print(">> End to write output")

def has_duplicates(array):
	return len(array) != len(set(array))

#global variables
all_class_array = []
all_enum_array = []

#collect the build service inforamtion with the latest file
def collectServiceInfo(lines, build_service_name):
	#Tidl service needs to be handled on different way
	isTidlFlag = False

	if build_service_name == "tidl":
		isTidlFlag = True
		class_header = "[HEADER]xxClass"
		enum_header = "[HEADER]xxEnum"
		receiver_name_header = "[HEADER]receiver_name"
		receiver_func_header = "[HEADER]receiver_function"
	else:
		class_header = "[HEADER]xxClass_" + build_service_name
		enum_header = "[HEADER]xxEnum_" + build_service_name
		receiver_name_header = "[HEADER]receiver_name_" + build_service_name
		receiver_func_header = "[HEADER]receiver_function_" + build_service_name
	#print("class_header : " +  class_header)
	#print("enum_header : " +  enum_header)
	#print("receiver_name_header : " +  receiver_name_header)
	#print("receiver_func_header : " +  receiver_func_header)

	#initialize values
	receiver_name_len = 0
	receiver_func_len = 0
	class_array = []
	enum_array = []
	writeFlag = WriteType.NONE

	filepath = warehouse_path + getLatestFileName()
	print(">> Get the information of : " + build_service_name)
	with open(filepath, 'r', encoding='cp1252') as csvfile:
		rdr = csv.reader(csvfile)
		for row in rdr:
			try:
				if isTidlFlag == True:
					#if the service is tidl, it needs to concatenate '_tidl' and add 'IsOn' vlaue
					if row[0] == class_header:
						row[0] = row[0] + "_tidl"
						writeFlag = WriteType.CLASS
						#lines.append([])
					elif row[0] == enum_header:
						row[0] = row[0] + "_tidl"
						writeFlag = WriteType.ENUM
						lines.append([])
					elif row[0] == receiver_name_header:
						row[0] = row[0] + "_tidl"
						row = [v for v in row if v]
						receiver_name_len = len(row)
						row.append("IsOn")
						writeFlag = WriteType.RECEIVER_NAME
						lines.append([])
					elif row[0] == receiver_func_header:
						row[0] = row[0] + "_tidl"
						row = [v for v in row if v]
						receiver_func_len = len(row)
						row.append("IsOn")
						writeFlag = WriteType.RECEIVER_FUNC
						lines.append([])
				else:
					if row[0] == class_header:
						writeFlag = WriteType.CLASS
						lines.append([])
					elif row[0] == enum_header:
						writeFlag = WriteType.ENUM
						#lines.append([])
					elif row[0] == receiver_name_header:
						writeFlag = WriteType.RECEIVER_NAME
						lines.append([])
					elif row[0] == receiver_func_header:
						writeFlag = WriteType.RECEIVER_FUNC
						lines.append([])

				#during writing, it needs to check it has valid value or not
				if writeFlag != WriteType.NONE:
					if checkValidValue(row) == False:
						writeFlag = WriteType.NONE

				#append value to class or enum
				if writeFlag == WriteType.CLASS:
					if row[0].startswith('[') == False:
						class_name = "[HEADER]" + row[0]
						class_array.append(class_name)
				elif writeFlag == WriteType.ENUM:
					if row[0].startswith('[') == False:
						enum_name = "[HEADER]" + row[0]
						enum_array.append(enum_name)
				
				if isTidlFlag == True:
					#if the service is tidl, it needs to add 'IsOn' vlaue (default : X)
					if writeFlag == WriteType.RECEIVER_NAME:
						if row[receiver_name_len] == "":
							row[receiver_name_len] = "X"
					elif writeFlag == WriteType.RECEIVER_FUNC:
						if row[2] == "1":
							row[receiver_func_len] = "X"

				if row[0] in class_array:
					writeFlag = WriteType.CLASS_MEM
					lines.append([])
				elif row[0] in enum_array:
					writeFlag = WriteType.ENUM_MEM
					lines.append([])

				if writeFlag != WriteType.NONE:
					#print(row)
					lines.append(row)
			except IndexError:
				pass
			except Exception as e:
				print("!!!Error (collectServiceInfo 1) : ", e)
				exit()

	try:
		global all_class_array
		global all_enum_array
		all_class_array = all_class_array + class_array
		all_enum_array = all_enum_array + enum_array
		if has_duplicates(all_class_array) == True:
			raise DuplicateError
		if has_duplicates(all_enum_array) == True:
			raise DuplicateError
	except DuplicateError as e:
		print("!!!Error (collectServiceInfo 2) : ", e)
		exit()

#copy the certain service from the latest file
def copyFromLatest(service, lines):
	print(">> Start to copy from latest file")
	start_str = "#===== START of " + service
	end_str = "#===== END of " + service
	write_flag = False

	filepath = output_path + "latest_tidl.csv"
	with open(filepath, 'r', encoding='cp1252') as csvfile:
		rdr = csv.reader(csvfile)
		for row in rdr:
			try:
				if not row and write_flag == True:
					lines.append(row)
					pass

				if row[0].startswith(start_str):
					write_flag = True
				elif row[0].startswith(end_str):
					write_flag = False

				if write_flag == True:
					lines.append(row)
					#print(row)

			except IndexError:
				pass
			except Exception as e:
				print("!!!Error (copyFromLatest) : ", e)
				exit()

#copy left services from the latest file
def copyLeftFromLatest(services, lines):
	write_flag = False

	filepath = output_path + "latest_tidl.csv"
	with open(filepath, 'r', encoding='cp1252') as csvfile:
		for service in services:
			start_str = "#===== START of " + service + " : Each Manager ===="
			end_str = "#===== END of " + service + " : Each Manager ======"
			rdr = csv.reader(csvfile)
			for row in rdr:
				try:
					if not row and write_flag == True:
						lines.append(row)
						pass

					if row[0].startswith(start_str):
						write_flag = True
					elif row[0].startswith(end_str):
						lines.append(row)
						write_flag = False

					if write_flag == True:
						lines.append(row)
				except IndexError:
					pass
				except Exception as e:
					print("!!!Error (copyLeftFromLatest) : ", e)
					exit()

#copy collected info to original csv file
def copyNewInfo(services):
	print(">> Start to copy new information")
	new_lines = []
	new = output_path + "Code_Generator_old.csv"
	old = output_path + "Code_Generator.csv"
	shutil.copyfile(old, new)

	start_strs = []
	end_strs = []
	add_str = "#===== Add TIDL Manager ====="
	write_flag = False

	for service in services:
		start_str = "#===== START of " + service + " : Each Manager ===="
		end_str = "#===== END of " + service + " : Each Manager ======"
		start_strs.append(start_str)
		end_strs.append(end_str)

	filepath = output_path + "Code_Generator.csv"
	with open(filepath, 'r', encoding='cp1252') as csvfile:
			rdr = csv.reader(csvfile)
			for row in rdr:
				try:
					if row[0] in start_strs:
						write_flag = True
						service = getServiceName(row[0])
						copyFromLatest(service, new_lines)
					elif row[0] in end_strs:
						write_flag = False
						services.remove(service)
					elif row[0].startswith(add_str):
						copyLeftFromLatest(services, new_lines)
						new_lines.append([])

					if write_flag == False:
						new_lines.append(row)
				except IndexError:
					pass
				except Exception as e:
					print("!!!Error (copyNewInfo) : ", e)
					exit()

	#os.remove(output_path + 'Code_Generator.csv')
	writeOutput(new_lines, 'Code_Generator.csv')
	print(">> End to copy new information")

#collect all information
def collectAllInfo():
	lines = []
	services = []
	getServices(getLatestFileName(), services)

	for service in services:
		start_str = "#===== START of " + service + " : Each Manager ===="
		end_str = "#===== END of " + service + " : Each Manager ======"
		lines.append([start_str])
		collectServiceInfo(lines, service)
		lines.append([end_str])
		lines.append([])

	#get the output
	writeOutput(lines, 'latest_tidl.csv')
	#for index, value in enumerate(lines):
	#	print(index, value)

	copyNewInfo(services)


#main function
print(">> The latest file is : " + getLatestFileName())
collectAllInfo()
print(">> Done collect")
