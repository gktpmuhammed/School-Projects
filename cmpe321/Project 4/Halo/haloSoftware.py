import sys
import csv
import time
import os


def createrecord(typename,filecount,liste):
    planet = "E226-S187"

    if filecount == '0':
        file = open(typename+'1.txt','w')
        record = liste[0] + " " + planet + " " + " ".join(liste)
        file.write(record)
        filecount = "1"
        file.close()
    else:
        file = open(typename+'1.txt','r+')
        records = file.read().splitlines()
        record = liste[0] + " " + planet + " " + " ".join(liste)
        for r in records:
            re=r.split(sep=None)
            if re[0]==liste[0]:
                return filecount, "failure"
        records.append(record)
        records.sort(key=lambda x: int(x.split(' ')[0]),reverse=True)
        filew = open(typename + '1.txt', 'w')
        for r in records:
            filew.write(r+"\n")
        filew.close()
        file.close()
    return filecount, "success"


def filterrecord(typename, conditionType, number, fieldIndex):
    file = open(typename + '1.txt', 'r+')
    records = file.read().splitlines()
    result = []
    if conditionType == "=":
        for r in records:
            temp = r.split(" ")
            if int(temp[fieldIndex]) == int(number):
                result.append(" ".join(temp[1:]))
    if conditionType == "<":
        for r in records:
            temp = r.split(" ")
            if int(temp[fieldIndex]) < int(number):
                result.append(" ".join(temp[1:]))
    if conditionType == ">":
        for r in records:
            temp = r.split(" ")
            if int(temp[fieldIndex]) > int(number):
                result.append(" ".join(temp[1:]))
    file.close()
    return (result)


def updaterecord(rid,typename,liste):
    file = open(typename + '1.txt', 'r+')
    records = file.read().splitlines()
    temp="failure"
    for r in records:
        if r.split(' ')[0] == rid:
            temprecord=rid+" "+"E226-S187"+" "+" ".join(liste)
            records.append(temprecord)
            records.remove(r)
            temp="success"
    records.sort(key=lambda x: int(x.split(' ')[0]), reverse=True)
    filew = open(typename + '1.txt', 'w')
    for r in records:
        filew.write(r + "\n")
    file.close()
    filew.close()
    return temp




def deleterecord(rid,typename):
    file = open(typename + '1.txt', 'r+')
    records = file.read().splitlines()
    temp="failure"
    for r in records:
        if r.split(' ')[0] == rid:
            records.remove(r)
            temp="success"
    filew = open(typename + '1.txt', 'w')
    for r in records:
        filew.write(r + "\n")
    file.close()
    filew.close()
    return temp

def searchrecord(rid,typename):
    file = open(typename + '1.txt', 'r+')
    records = file.read().splitlines()
    for r in records:
        if r.split(' ')[0] == rid:
            return "success",r
    file.close()
    return "failure",None


def read(input, output):
    inputfile=open(input, 'r')
    lines = inputfile.read().splitlines()
    f = open('haloLog.csv', 'a', encoding='UTF8', newline='')

    for line in lines:
        control = True
        systemread = open('systemcatalog.txt', 'r')
        systemlines = systemread.read().splitlines()
        writer = csv.writer(f)
        l=line.split(sep=None)
        log = []
        admin="admin"
        log.append(admin)
        log.append(str(int(time.time())))
        log.append(str(line))

        if l[0]=="create":
            if l[1] == "type":
                typename=l[2]
                if len(l)<4:
                    log.append("failure")
                    writer.writerow(log)
                    continue
                rang=int(l[3])
                if len(l[4:4+rang]) == rang:
                    for sis in systemlines:
                        si=sis.split(sep=None)
                        if si[0]==typename:
                            log.append("failure")
                            control=False
                    if control:
                        systemlines.append(' '.join(l[2:])+' 0')
                        log.append("success")
                    control=True
                else:
                    log.append("failure")

            if l[1] == "record":
                typename = l[2]
                count = -1
                filecount = ""
                for sis in systemlines:
                    si = sis.split(sep=None)
                    if si[0] == typename:
                        count = len(si) - 2
                        filecount = si[-1]

                if len(l[3:]) != count:
                    log.append("failure")
                else:
                    filecount,stat = createrecord(typename, filecount, l[3:])
                    for i in range(len(systemlines)):
                        si = systemlines[i].split(sep=None)
                        if si[0] == typename:
                            si[-1]=filecount
                            systemlines[i] = ' '.join(si)
                    log.append(stat)




        if l[0] == "inherit":
            if l[1] == "type":
                stat="failure"
                if len(l)<4:
                    log.append("failure")
                    writer.writerow(log)
                    continue
                oldfields = [l[2]]
                oldtypename = l[3]
                for sis in systemlines:
                    si = sis.split(sep=None)
                    if si[0] == l[2]:
                        log.append("failure")
                        control = False
                    if si[0] == oldtypename:
                        stat="success"
                        oldfields+=str(len(l)-4+len(si)-3)
                        oldfields=oldfields+si[2:len(si)-1]
                if control:
                    oldfields+=l[4:]
                    systemlines.append(' '.join(oldfields)+' 0')
                    log.append(stat)

        if l[0] == "delete":
            if l[1] == "type":
                typename = l[2]
                tmpp="failure"
                for sis in systemlines:
                    si = sis.split(sep=None)
                    if si[0] == typename:
                        tmpp = "success"
                        fileNum = si[-1]
                        for i in range(1, int(fileNum) + 1):
                            tempstr = typename + str(i)+".txt"
                            os.remove(tempstr)
                        systemlines.remove(sis)
                log.append(tmpp)

            if l[1] == "record":
                typename = l[2]
                rid = l[3]
                stat=deleterecord(rid,typename)
                log.append(stat)

        if l[0] == "list":
            if l[1] == "type":
                outfile = open(output, 'a')
                if not systemlines:
                    log.append("failure")
                else:
                    for sis in systemlines:
                        si = sis.split(sep=None)
                        outfile.write(si[0] + "\n")
                    log.append("success")
            if l[1] == "record":
                typename = l[2]
                tmpp="failure"
                outfile = open(output, 'a')
                if os.path.isfile(typename+'1.txt'):
                    file = open(typename+'1.txt','r')
                    reader=file.read().splitlines()
                    tmpp="success"
                    for r in reader:
                        re=r[len(r.split(' ')[0])+1:]
                        outfile.write(re+'\n')
                    file.close()
                log.append(tmpp)

        if l[0] == "search":
            if l[1] == "record":
                typename = l[2]
                rid = l[3]
                stat,rec=searchrecord(rid,typename)
                log.append(stat)
                if rec is not None:
                    outfile = open(output, 'a')
                    re = rec[len(rec.split(' ')[0]) + 1:]
                    outfile.write(re + '\n')

        if l[0] == "update":
            if l[1] == "record":
                typename = l[2]
                rid = l[3]
                stat =updaterecord(rid,typename,l[3:])
                log.append(stat)

        if l[0] == "filter":
            if l[1] == "record":
                typename = l[2]
                condition = l[3]
                index = 0
                conditionType = ""
                if "<" in condition:
                    index = condition.index("<")
                    conditionType = "<"
                if ">" in condition:
                    index = condition.index(">")
                    conditionType = ">"
                if "=" in condition:
                    index = condition.index("=")
                    conditionType = "="

                fieldName = condition[0:index]
                fieldIndex = -1
                for sis in systemlines:
                    si = sis.split(sep=None)
                    if si[0] == typename:
                        for i in range(1, len(si)):
                            if si[i] == fieldName:
                                fieldIndex = i + 1

                number = condition[index + 1:]
                results = filterrecord(typename, conditionType, number, fieldIndex)
                outfile = open(output, 'a')
                for re in results:
                    outfile.write(re + "\n")
                log.append("success")












        writer.writerow(log)
        systemlines.sort()
        systemwrite = open('systemcatalog.txt', 'w')
        for sl in systemlines:
            systemwrite.write(sl+"\n")
        systemread.close()
        systemwrite.close()




if __name__ == '__main__':
    if not os.path.isfile('systemcatalog.txt'):
        systemwrite = open('systemcatalog.txt', 'w')
    read(sys.argv[1], sys.argv[2])
