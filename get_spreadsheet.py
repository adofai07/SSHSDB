from MoveSeat import get_values, EESUK_SHEET, EESUK_APP_RANGE

eesuk_vals = get_values(EESUK_SHEET, EESUK_APP_RANGE)

for i in eesuk_vals:
    if len(i) > 8:
        if i[0].isdigit()