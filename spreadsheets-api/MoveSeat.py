from __future__ import print_function

import functools
import os.path

from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from googleapiclient.errors import HttpError

import pandas as pd # for further analyzing the sheet
import numpy as np

# If modifying these scopes, delete the file token.json.
SCOPES = ['https://www.googleapis.com/auth/spreadsheets']

EESUK_SHEET = '1pk3xJdqa2y9xDR2B7LcdvcmwCt-mlE95upEC6mIObwc'
TEST_SHEET = '1mZU9i-pnEsoB9vKBWPv4iJFKsOC-8OXWuGonGL2yd40'

EESUK_APP_RANGE = '학생 신청!A3:P'
DORM_APP_RANGE = 'RawData!A3:O'
TODAY_APP_RANGE = '사전퇴사신청!A4:AJ'
WELCOME_APP_RANGE = '안내!Q3:V30'

options = {
    "null" : "",
    "kick" : "",
    "home" : "",
    "stdy" : "",
    "sem1" : "세미나실1",
    "sem2" : "세미나실2",
    "sem3" : "세미나실3",
    "sem4" : "세미나실4",
    "libr" : "도서관",
    "emp1" : "1학년공강실",
    "emp2" : "2학년공강실",
    "emp3" : "3학년공강실",
    "eng1" : "외국어강의실1",
    "eng2" : "외국어강의실2",
    "eng3" : "외국어강의실3",
    "med1" : "미디어부스1",
    "med2" : "미디어부스2",
    "scix" : "융합과학실험실",
    "geox" : "지구과학실험실",
    "meet" : "대회의실",
    "hist" : "역사관",
    "tech" : "첨단강연",
    "dorm" : "(기숙사)",
    "vrrm" : "VR실",
    "sghs" : "국제고",
    "misc" : "(기타장소)"
}

revop = {
    '': 'null',
    '퇴사': 'kick',
    '미입사': 'home',
    '자습실': 'stdy',
    '세미나실1': 'sem1',
    '세미나실2': 'sem2',
    '세미나실3': 'sem3',
    '세미나실4': 'sem4',
    '도서관': 'libr',
    '1학년공강실': 'emp1',
    '2학년공강실': 'emp2',
    '3학년공강실': 'emp3',
    '외국어강의실1': 'eng1',
    '외국어강의실2': 'eng2',
    '외국어강의실3': 'eng3',
    '수학강의실1': 'mat1',
    '수학강의실2': 'mat2',
    '수학강의실3': 'mat3',
    '물리강의실1': 'phy1',
    '물리강의실2': 'phy2',
    '물리강의실3': 'phy3',
    '화학강의실1': 'chm1',
    '화학강의실2': 'chm2',
    '화학강의실3': 'chm3',
    '미디어부스1': 'med1',
    '미디어부스2': 'med2',
    '융합과학실험실': 'scix',
    '지구과학실험실': 'geox',
    '물리실험실1': 'phx1',
    '물리실험실2': 'phx2',
    '물리실험실3': 'phx3',
    '화학실험실1': 'chx1',
    '화학실험실2': 'chx2',
    '화학실험실3': 'chx3',
    '대회의실': 'meet',
    '역사관': 'hist',
    '첨단강연': 'tech',
    "(기숙사)": "dorm",
    "국제고": "sghs",
    "강당": "adtm",
    "시청각실": "thtr"
}

def revopfn(x):
    if x in revop:
        return revop[x]

    else:
        return "NONE"

def get_cred():
    """Returns the creds and also makes token.json from credentials. **REQUIRES credentials.json**"""
    creds = None
    if os.path.exists('MoveSeatToken.json'):
        creds = Credentials.from_authorized_user_file('MoveSeatToken.json', SCOPES)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                'MoveSeat.json', SCOPES)
            creds = flow.run_local_server(port=0)
        with open('MoveSeatToken.json', 'w') as token:
            token.write(creds.to_json())

    return creds


def get_values(target, range):
    creds = get_cred()
    try:
        service = build('sheets', 'v4', credentials=creds)
        sheet = service.spreadsheets()
        result = sheet.values().get(spreadsheetId=target, range=range).execute()
        values = result.get('values', [])
        return values

    except HttpError as err:
        return err
    
def get_dorm_values(target, range):
    creds = get_cred()
    try:
        service = build('sheets', 'v4', credentials=creds)
        sheet = service.spreadsheets()
        result = sheet.values().get(spreadsheetId = target, range=range).execute()
        values = result.get('values', [])
        return values

    except HttpError as err:
        return err

@functools.cache
def get_dorm_values_cached(target, range):
    creds = get_cred()
    try:
        service = build('sheets', 'v4', credentials=creds)
        sheet = service.spreadsheets()
        result = sheet.values().get(spreadsheetId = target, range=range).execute()
        values = result.get('values', [])
        return values

    except HttpError as err:
        return err


def process_value(values):
    tmp = pd.DataFrame(values)
    tmp = tmp.rename(columns={0 : "no", 1: "name", 2: "loc1", 3: "cnt1", 4: "loc1r", 5: "loc2", 6: "cnt2", 7: "loc2r"}).copy()
    def format(x):
        if x == "":
            return 0
        return int(x)
    tmp['no'] = tmp['no'].apply(format)

    return tmp

def change_cell(target, range, val):
    creds = get_cred()
    try:
        service = build('sheets', 'v4', credentials=creds)
        sheet = service.spreadsheets()
        sheet.values().update(spreadsheetId=target, range=range, valueInputOption="RAW", body={"values" : [[val]]}).execute()
        return True

    except HttpError as err:
        return err

def get_vertical_location(name_or_id):
    if type(name_or_id) == str and name_or_id.isdigit():
        name_or_id = int(name_or_id)
        
    sht = process_value(get_values(EESUK_SHEET, EESUK_APP_RANGE))
    if type(name_or_id) == str:
        return list(sht['name'] == name_or_id).index(True) + 3
    return list(sht['no'] == name_or_id).index(True) + 3

def _move(name_or_id, idx, place):
    if name_or_id.isdigit():
        name_or_id = int(name_or_id)
        
    v = get_vertical_location(name_or_id)
    t = "C" if idx == 1 else "F"
    loc = F"학생 신청!{t}{v}"

    change_cell(EESUK_SHEET, loc, options[place])

def move(name_or_id, idx, place):
    v = get_vertical_location(name_or_id)
    t = "C" if idx == 1 else "F"
    loc = F"학생 신청!{t}{v}"

    if get_values(EESUK_SHEET, loc) == [[options[place]]]:
        return False
    else:
        change_cell(EESUK_SHEET, loc, options[place])
        return True

def get_name_12_pos(*numbers, margin=2):
    sheet = get_values(EESUK_SHEET, EESUK_APP_RANGE)
    idxs = []
    ret = []

    for num in numbers:
        for i in range(len(sheet)):
            if sheet[i][0] == str(num):
                for j in range(i - margin, i + margin + 1):
                    ret.append([sheet[j][0], sheet[j][1], revopfn(sheet[j][4]), revopfn(sheet[j][7])])

    return ret

def get_place_people(pl, prd):
    sheet = get_values(EESUK_SHEET, EESUK_APP_RANGE)
    ret = []

    if prd == 1:
        for i in range(len(sheet)):
            try:
                if revopfn(sheet[i][4]) == pl:
                    ret.append((sheet[i][1], int(sheet[i][0][0])))
            except:
                ...

    if prd == 2:
        for i in range(len(sheet)):
            try:
                if revopfn(sheet[i][7]) == pl:
                    ret.append((sheet[i][1], int(sheet[i][0][0])))
            except:
                ...

    return ret

def get_groupby_place(prd):
    sheet = get_values(EESUK_SHEET, EESUK_APP_RANGE)
    ret = []

    if prd == 1:
        for i in range(len(sheet)):
            try:
                sheet[i][4] == sheet[i][4].strip()

                if sheet[i][4] in ("", "퇴사", "미입사", "자습실"):
                    continue

                chk = False

                for j in range(len(ret)):
                    if ret[j][0] == sheet[i][4]:
                        chk = True

                        ret[j][1].append(sheet[i][1])

                if not chk:
                    ret.append([sheet[i][4], [sheet[i][1]]])

            except:
                ...

    if prd == 2:
        for i in range(len(sheet)):
            try:
                sheet[i][7] == sheet[i][7].strip()

                if sheet[i][7] in ("", "퇴사", "미입사", "자습실"):
                    continue

                if (len(sheet[i][7]) == 4 and sheet[i][7].endswith("호")) or sheet[i][7] == "기숙사":
                    sheet[i][7] = "Dorm"

                chk = False

                for j in range(len(ret)):
                    if ret[j][0] == sheet[i][7]:
                        chk = True

                        ret[j][1].append(sheet[i][1])

                if not chk:
                    ret.append([sheet[i][7], [sheet[i][1]]])

            except:
                ...

    return ret

def get_dorm_info(kw, sheet=None):
    if sheet is None: a = get_dorm_values(EESUK_SHEET, DORM_APP_RANGE)
    else: a = sheet
    
    ret = []

    if kw.isdigit() and int(kw) > 1000:
        for i in a:
            try:
                if i[0] == kw:
                    return F"```py\n학번: {i[0]}\n이름: {i[1]}\n기숙사: {i[2]}\n입사 유형: {i[3]}\n성별: {i[4]}\n```"
            except:
                ...

    if kw.isdigit() and int(kw) < 1000:
        for i in a:
            try:
                print(i)
                if i[2] == kw:
                    ret.append(F"```py\n학번: {i[0]}\n이름: {i[1]}\n기숙사: {i[2]}\n입사 유형: {i[3]}\n성별: {i[4]}\n```")
            except:
                ...

        return "\n".join(ret)

    for i in a:
        try:
            if i[1] == kw:
                ret.append(F"```py\n학번: {i[0]}\n이름: {i[1]}\n기숙사: {i[2]}\n입사 유형: {i[3]}\n성별: {i[4]}\n```")
        except:
            ...

    return "\n".join(ret)

def get_sunday():
    a = get_dorm_values(EESUK_SHEET, DORM_APP_RANGE)

    ret = "```py\n"

    for i in a:
        try:
            if "일요일" in i[3]:
                ret += F"{i[0]} | {i[1]}\n"
            
        except:
            ...

    return ret + "```"

def get_today():
    a = get_values(EESUK_SHEET, TODAY_APP_RANGE)

    ret = "```py\nIf you only see this message there are no people\n"

    for i in a:
        if i[-1] == "O" and i[0].isdigit():
            ret += F"{i[-3]} | {i[-2]}\n"

    return ret + "```"

def get_reset_time():
    a = get_dorm_values(EESUK_SHEET, DORM_APP_RANGE)
    
    for i in range(len(a)):
        for j in range(len(a[i])):
            if a[i][j] == "리셋 시간":
                return a[i + 1][j]

if __name__ == "__main__":
    print(get_values(EESUK_SHEET, EESUK_APP_RANGE))