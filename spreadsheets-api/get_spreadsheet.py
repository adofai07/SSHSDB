from MoveSeat import get_values, EESUK_SHEET, EESUK_APP_RANGE, revop

def is_good_name(x: str) -> bool:
    if   len(x) == 3:
        return (
            '가' <= x[0] <= '힣' and
            '가' <= x[1] <= '힣' and
            '가' <= x[2] <= '힣'
        )
        
    elif len(x) == 4:
        return (
            '가' <= x[0] <= '힣' and
            '가' <= x[1] <= '힣' and
            '가' <= x[2] <= '힣' and
            'A' <= x[3] <= 'Z'
        )
        
    else:
        return False

eesuk_vals = get_values(EESUK_SHEET, EESUK_APP_RANGE)

print(eesuk_vals)

sep = " "

with open("./sheet.txt", "w+", encoding="utf-8") as f:
    for i in eesuk_vals:
        if len(i) >= 8:
            if (
                len(i[0]) == 4 and i[0][0] in "123" and i[0].isdigit()
                # and is_good_name(i[1])
            ):
                print(i)
                f.write(F"{i[0]}{sep}{i[1].ljust(4, ' ')}{sep}{i[3].zfill(3)}{sep}{revop.get(i[4], '????')}{sep}{i[6].zfill(3)}{sep}{revop.get(i[7], '????')}\n")
                
