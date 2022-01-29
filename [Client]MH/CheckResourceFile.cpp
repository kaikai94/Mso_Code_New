#include "stdafx.h"
#include "CheckResourceFile.h"
#include <fstream>
#include <WinInet.h>
#include "MHFile.h"
#include <io.h>
#include "./lib/ClinkAPI.h"
#include "GameIn.h"
#include "TClient.h"
#include "Protocol.h"
#include "MHNetwork.h"
extern HWND _g_hWnd;
extern BOOL   g_IsExit;
//[安卫士秘钥定义][BY:十里坡剑神][QQ:112582793][2019-5-5][23:41:24]
std::string ClinkKey1 = "QQYAAOPV1elQ7ScAAJEdm+SSkVYzuGKXTrSuXXbCp0gduzDxMafslWdvVSnLtS0RJjG/khYkgRs1GqMuJ3hzjMXySbFgKmk9pfvuwmM4y5eHnTZFAhUy9zkVuxcDvW7t+8dj12t7sPrGgusaNL23/E0MpohBLomvwFF3Sm1SOueMAMaa1PMsC+w6fX8oF8N7Xb7I9hbkrMw9shYaBGWMdEgEjDP2HCdZ3pqbBq8KhqiM++yuiELRp1P4XSHcEgeGrQg3JIqPg0CXbhh6SKYcMyxtjAh33eoFMuPf/L4GfMuTFFin0mEd/HerFxoqVl6s2yn0b4T+D84cMfGmroaBkPBL5TRNdF/JT4+PysbQ2dqEMyCUEA1GlAWIiqBvdk3vKoUp3m8qXmfJKyD0bkMIS1cXIXpl7IDjBFEvxOi+WddBnqBBp/lqWtS4yuTsUSXBgD9JWFXSij0FRNod+ph9CtBHs86hxaJcq8WlqyGa9bIsbEOodQGBbhRjPGIfTV87Or8TpJ1VH7mb66yOLd0baU4o8I1U05TPxES6PYlAPV1maXBhh1EV1k2JDYlU8kE1SNIT9VzR5mYrQ95YmuEwW61OKMJsueD8ExpAysYLe9EDhPS2LEYBk4EKNURxuztqWbHaBcl9yI7wFuJCkRueH8cMVBXseC5mTJNBYoOci1bd1HEVL3UIhLtlxRZjvgJmahV8ZGYmfd+Cc7c1PGYKU4qLZLuoie9RBM9ZhrIYA42R2fZtICbutlaRkbu80UhVW3GK1lpFvOAlKl+WzPYsam8qo2FvmzJkm6xlF/czkzGGBCpAjCwzSxwVR/XFWOleZCZLki7l5b4/Jjqs36Xn8ZUuEMjM69xD9VhFWVQ8nUPBB7LQ5wEDoB1rAWbwhWp2LVVfl0ghkXdJB5zUoIxqAof26djyFwqUKm6qYOp1Um/szSz93Ab6oHQ6h99kLf6QAP3d+ZqRgxVLyWzYdjCM0rQs59FfbCWB+kljuI8pMPtvGWDrl1jTW0oWsRKYISlDrLQqN0UtMJPYXP1QLcqFIZlwaV3PnztcV4uHSbkT6NggHwva/ve1QufLEzpZgy1/j5B3J7fLcw14zd+5pObZxHIkMA2+qiKhuSd1BIsAAtNeCLgpmsAhU6v9YTEY+5wA8U3HNVIfv+Rt1WyQVm+AeMoMWpwuryqwA9thwEnwib2JdBE+OMyBP0fTusRTj/w8fehlC6H+0UEjFjJ1vf2bfEyuBuBdQkD9ihtVWvNkXHLeJENgin0ciqHOGiGZC4FJNz54pIkc3TunZ4CFqXdMvri/lpi5O/DNkns+jSceY36ZCyK167fowkYItyL7leAqglpn2XTnjvtMqRIO3E8TiQQC651VltIB1tNRsYLN0fuZFAvMtYgKsHwlaUGl2uT19C4e+KhC0+uHZgOfldx5fdYVc8dERtJ69zshMCyQWvrtW7tUr9dSVz8fRE+EaRqeq+tpkGYcsa5aowFtIaO+9gsHsTohJM3+wRuIJAGD9I0hgoRAJsmT0KkN+QzW5DaaYE1DfbfefYYeas1Lk4ys1q1yTJ0ffs9Bct+PEyrAKNzk7MOzQHfVQQT8Irv7SD0dXyJeY5eXeuB9AQrXT2lbXELvZaXH+JY1nAuZIlsTRCmhcrMHYsRETXLhNj5t/PCaoMV5C+KXVGaLKMWM/ViFabzfNty1VPh9qhgz5MFYxp8Z/oCVmTlGCh1Mezn9t08ZXMyAjcYU40zL+1/KZG6kM2Emc/3W7s+CmCu+aNe+fvLIGYj01jnXBBo0yZ3Ye6Stp0jX10NQQm37/POaVXpzDHxsAkrlrlwoTW7joSUyqR2GEcCP5qaud9Ub99hEPK6ev7wga3hZxvfrr4nUa59HyVPVFES54yXVvI";
std::string ClinkKey2 = "l0yrWC4hG/ROFnET2jLEnHozYc2Qw20i2EbSdq5NuWSa7u5PtCaMt4FCe937cVMuf58EDuueF8PfeH/HToVV8F0MeOs0ZNhTppo8TkePJ1bzs/b/Kbp4/LcacDuWpXjr7Gtw7vhn2grGsFhBUcl8QFEo05qeRraSg4wUKeScvPEhUGHb/jhOi9ncKGlSJ5rtji/abcHeTZWk3ltsM7AmOfNXIAFnoEII2jewsi5NIFvhJou0ok8fQvLMFy/c0kgptf5mCveJde3twZOKDYtnV63/ew0UYYekJ8z+m3kSK+f/hvwv1xVUV5CdttC7lTVXRL9h3Uc69URmseMqetpU25WbinafN3CIqyNsdpqm+HKXp0bJkF6QxJGnuRUyKcy2OOsrEBquuA0u6SIjzm6Zv7lQktODmytG+sXk535bNnFWpk2fa9hn6N85iqccbOYjDelbwZbpIq2/xBVEycCOw6KgzRgRCkPg+JnkSfCEKn3PsemE/1TqbWOv34ezEx8Oqqb0e6/N8fOletjs5OidK6mXdQfW+cQ9tl+UYRqsLcWK1Y9Jf67N4b/V33SL/WiLAqPuC54HJ6DSxqVQOYDo6TzzUg9JM7vqyXNH8SpPocjXGV5mzgI3c73eZrj5bbgu/NZHeJtpIc7X40IavyaGgwTseQAyYisqy95Berzy8Vu7GOQz+CkdbeePfsrAHKSHR+OkDOSn7Iq7DKytB6cB5PHmX5YyUorLzwsYktNAWr8pyiLZTtRXAyxJeOt8YFYwjYnY6a0ONPVYyD3Zj0mnarHtcz10KJYdf0xp2NmfT06k2BVJZ2ECG4U9YHaWbXPSkg4Znu0YvETzy1pQpGwbuXjfJKc0beNA21eoRrgfZwHYlzbLZbBhnBuwRawGoXcZEbb9pzjcjiQa2LOFYGWcxERId/0VQEPIfZh7ZBg3OinQjyTxIb/I+/kCsJzszBumE3FXvhlp88FjJTmQURoLw56JpNEwm5KUf3v0Eo+my6F8tVCm32vwdotTnfx2OJre5uzuUyc6dXfOs06dZdExvEmIE5u5ZOQiiT5FzP2WHjbhJRuSuFh6Mk/IyBwtQvCqIeGi7gUPBYH4y8no5Kh6KJUHrDDFjuPxDdFL4Jy35TY9m5sb8yAh0g95qex+gU2HXp9nFZa/1JLtkeAzsyTlINaTu0j9ThV05doxO9UHYmyDtIH9MM+PfacIPkE/3h7KhHkF6tXkmgB58j+8bpit1j9zkA6S99AXBC2R5R+0NPerVqdr8YeGrxpxc06Psw8OWkgRvD4QN4yI/saoQqTgtc0oEQfhMeS37ODUEYSRh0Euc4q3oni5ey8K0wKg3kx1gM1hV5H3HRhjab9NPAYH52eFib8KKPNUfoXY1l7SBx6EXoDHIC+fF2ZiREipXD8JAuuLC0SdW1e6HMkMDia5gVKgyO11QMc35lJYp9S6mxJWz8NzRuvJ9vMpDPNWgwkvmLLl+JNRg7IUQyTiNLNy/2GmpU5TRjpmxrHqTCPD4VNJGP8R1JJ+9TwBXvPG17I/pg2Br926zFJ4fyHRZzd+QFHBqQu0USrPJ6ASUecmr+7/qpYmIjF1jkyMmVVgnZ3kgw6h9T6We1Tisng9mGp7Md8i0ayRWB4QXAq/F6fFXonyHb7IdKqgWT8hM3+5N9XSEWeGPbvxUyrQgakT50O9HUPuK3qrks0UcohdUBSuVwQ03QYwTxDd8MqD9/MBnFH7dBYbyvxLJm6aUbsjk/MWMiqE7wotbN4jEDqwF6rKAuYWYdv+cov32Zanld1lmmy3faXIbK03zgc36LrBix6z0E61DRIXB6+ejPVzdcbwvXW9+Ink8aY43pPEtpmxJl5n08WOoI06oGYGGd+XxEgwNys1E4KU3s6YfC";
std::string ClinkKey3 = "5+MlEF7VaW+5HxcgzoY0gT7se2gQpbhI9ubALL0wT21iROvG4E8Dxo8MimPrUePJBKYO/m4esUIr3Bl2FT2xiM+ovYqZbZ/t6LPXBBpUW6UC9SgWIU/+Q5GpmwQ2jLFSWgKTQpJZL5UGIBW5Rq0n1YBycx4UmmZ/qXr379Gbt1wChakYwBkSp8aRDwE5RfzigsSDwhiTPb6aE5gYYD/Dami4tsAEDhVmX8pXgopkRmN9FGwMF/NEbUfpfKyA+UeGzKOYgI083nOvHGNMO7o0Nsw0mkvRt1NzdEh4GZBn5WWDz7TlysTVQTX4W/qQ9jKMtUbfS176sxVQJAtEwsTiSDX8x2sFTWoHXlkW2FtY4nwW62NKhpkGiBuflaa4kH1vFRPEki7Fhmjb1+wF5Nq490GOLdVhl8Ca9T9PZbbDTczapcJIkk9OUGX6bq2sAaNA5ZxkKmOmDn1RKZzudiGwF4ogKT4XcGTwP9xwZikT7fSbD34aXiNpNJdF9WmyX9hDSVYjjNZkKHU4cumFNadTPjqnUmehlrhmtfSm0Xr6Pt8DLRzS7mEdNuUJ4Oo44v3v8bcuCwxxoLKdu9XhvVb6PDQKgQiKgE7YiRyAmLTMLtH1tt/ByzPTkYtoNTOLO3eODqS2LWH1ZHBoVYDv7wj9z40E6gd2OHABNu97IUobTU6j8IoQFgBwAFFuWHWx46CDamSlh09wGNQ0tfZweUIlTQgyS/187bmMkuq0CN75TlJP97bldcX9M9i022AJAIYPSAptfpehBnyqGPNkuhD1OWakIIGLP9dCMxclxxJYg5Jjpb7Ti865rPA5OlzQmXjHvqH4Y94FgwgyKkp/ez5qUZRhABRQIa2W6XOPUQV7cJmeXQhvXx+yXiK2oWCDOTCxzujGb5hjdp3zYnhrdDIDtbiw4B41+fF0raDxmX5MEq9C3IEcswEBOuVqe/ymR+nvsuqn0lgSe9T1UsD2cdwep0/HKZav5gK3rqU4yn30zWW2AuWTpD5XL2CrJWE/CcgJTNOs3R5SP8vo7iSy2LBXAYNbQG71iQMQcPg1A8Ggd6XbL32oR+5HY8EaqrQao1avVCTc48hZEsHQm20lHTsKR3d+1akzrbiUE9yr2t5O9cuiha6dwpLcgRy616uBIkaGEGI8msyre6ZqSNDqrDfigqSggZaMbIPjoVONd9UMEvCVJcp7EVr+9QoIMnrB8NPk11QG4J+v+KPdV/h4HgTQXTXeVIdrZnFJoIuMhDkJIrB+qJ+ABTFQ5SsnnbhhhdYFNtNF4ZtMeHDiuz8TInRp1DVI+XVjT1gEjm5bRMi35xu0hk1lwz8UMi8vnEnwPMK2vceCGIthoeV7QFMt7zSmK9IjXpsmBBD1NMZFNLIPElGqpIUANc+PUuNm6ILq5kH6iHF1zd0nj7m3jOPDG20zbC4CgkutpfUDkwu2vxqNCkyFSqpBmERzghXLSKfryzd2M3xU6GT0TOF7cTnRb8+eKYGWPc/B8NLojf424FxNF8fTP4ucOCDnj/3IxScKoBJELR8mVI1ntEGoa1plvmuFLVpx/S9DAfBhrqzR+Cur3ezr8LM9J8E5a4pK8szEgtCSQpfLl6HhASstw6XflLoW/IxteMy2RwaXN5Gk+A0kmyzbP7WS3kVKCe6HEGIcWdbOM/U/VwkE2vqQM18QPQgdCCh4A1UFgh5GIm/cLjeje9261MQX40Qi12O6AqrlOsrmagc5olWAybDW+DSZJqvC7LeBVGhCBSTjVivxSoBzYOej9MMwadeHOgsUQOoPiLC39hTxY+RipGThVUwqjn4wY5dM9TDnkYLe+RpdLsQ40816OlNrgPwCQFBeTROLXauL49tSwpGAzmSRE2nKBXWfqYF9rlQrxOKaSb";
std::string ClinkKey4 = "TeiURvUptu0Urn5BA7+Grt2W2SNzvZvaOQDa1XXx74t3ncoEUgIb/MXDMrtsNxQrXoPep0BxF5BMqg3UTtF3Er7i2VXlglzbetxdoIrgfeH8r3bSIzg69nY5t6f0N63UXAAMaW2uNcw28yxkE2cVXCwL559ahP9mt9Lfq7qU1zMuaVRq0/R0PmmqixALtuMfFGtr2DAOebcQhbGazooBkhvZNZoCqmg3h6kVoAnPWuD/sIdfKP7jL9+dSk4i6V/fmOkEEnFJDGnjJECW6NmtMSrAqw3yomkM5hWqZVAaBK45NgcAyzhvlqQdbASuXDcF4AkjYk5B7PaP5yxLnpcZyR+STZLSrHXSUPRZ1TZEbnnehHVG5qJf50jwcMQMA/gM85LlDA6EMzNzQQFJ5/fAO4iifPoRwuJ9MlZJwiux1jzsTNGTX0JrSlEx05WRFg9BMEU7WZb96zCwd8m4OlEXsXq0TYsWLNw2NxKo3+9MzCw2+IHmhmyV/lGg4ud4kw/gKgIa3LhkKwNo5RKERQsxU5udYTHfITsOHk8pdUnkvYQCuLXdIf9j0t1+fYFKEblGX3872yHOAOjXDK5ZKqW5ZDeOYZxFZCGsKZ5UQLOptAyV0xXKqNJiRJfM9hUxCpgXxQlZu5Ac8PtNj6GDWcAlPUihw1ef7qtVHBzkeF38Wy7mTHuaP+hceNIqbQkg3trHl8jMjydAh8ZN9J9zGIiNQSRynQ00iu1ARUp1ZYGiJE0yLMCFL8vnlcS1XDtY2VBZjQvUriHNcxzbbDncywJEq8BSMPun+BFYivdIjx6R92vqK9hlvXYLZKmjqbOCGrqiYKB69qhkkYT6jBM6dPPuKXTd4q8pzu46RrxB9F5dtnA8lh5+JvDDdYpDeyFiwP+VtWA+lBgbaJGyIL/cEp0mnLWjDd7FYAKEGm0U5lPM13XMnC7eHxJYAS18y/Yw11LtDM5bv6MQwFVtqoma9oI95Uk6H2ZuSG2TSvyxWJP8MzMaJ8l1UReyu8vZYGSIvQqLPtas9rfsfrRgyHS2X2exYWdGgM64qIQxd2OqdNf5Co3SJgmtTzx7Laj1FJiTDFxEGA79MixLspZCe49EMfncV63W1Cld9BIqy1MFs5P7jmsHRmzI+8hRhlNzasO9Fuu/GKzarCgi5hP8wmbJRg7yVu0ETNuNA2vVhU216plObaRnNprXANJC1FBIzBlRiDBUYQPWmMnwMgVBBSbaXPtqnlSyUg9UG0YKwXiLNDcyc/z9YXmFZ8TaZwSVlfurWGdU3i20ryYw4FW9u7dRdP29/oEp3Otx5TaP1+rqznVKX2jxJlYJbcTGvHEj15v9aOIWx4dqgXtr0ujnNLdLRgHAY2xOQF6HpJC8QyBqTg2Kcm7o0/b9373thTN9EjkNe2Z/Q2QvrxiTjxL2ri5LAZTbTKJYQnIKwnOuUMsLGoJXrse56vqZWsNGVUTUBzGPgoOugcWGpSjkZHDwgV5NeGSQnu9WkRaKMeHvAf0dJssiHr43bHjMwPmetXZ5qNl06MzQQ2hCKPFtBF/Dl5M7xz2VvmKABhvbs83lt9uQEIkIahkQMGak6Qkb4VEWvo5/2gXyyaOcslK7kURYgelAFocXkjMWQ2EokgmaXnxvRUyVv4kB9W/buyOyk9X7E8AZuNRFwNgN2rK7+Os5t/f2ZLMaFFHayswLpLqHjXmEwR4IfXg50rgBSwbRWprugNCFhxbV9OFFJhkyynybk6sfESY3D/PIcrTN1TGqKW+rcm2bJuRxs5sK/4LptDwW/bWuwAQjwe4KtPNTPJqnFNyJwkPvHq4r+fFCx5N3iuaIosssPaKvfnncMQ2YIIVh6nbAMlgxnv7OKJvHbS4f8Du1XdnYd7JLqmfzsZrRA6";
std::string ClinkKey5 = "Pcj74Qd43LKvYfwkH85pHPMDki/93Fyp7L1qrenWfvb7HB6XJ1eO1rEkgu+SmW8C9PYcqrnxkvHefljXqY4o6eF3zs9T6W9Tes465ZdegWpalphouN+86baCIiwOA3u4VIFc+irHNIg31AIKRuN8j/PeaA42sJUiBqSUBgfO7jKoDsLuNwfEIdjcDby/MXhs+HBTKVvS8F8bfcJWk62clWvU8/1aJUdUgAyn8qHZkB2qEE581kkvpARaIx2CdRBEW30Qt1J80Cok0tAUQcMPIK/tLwY/8ekjO7EAgIizKpBpzuck4v+mNWPZtOQd9fg41H6BQnqmkKrVgKBiFz6b8tS//p9QGWY0M7XoPxS/kFnOG6f6bG45RNst2eCE8rWf8KuvGR37SzNeMzBgD/A954Ai6bikLlsMQ6ct/KmozJB2jAIZ2l8DTjzjzU39CdFwKL1QrNFZtbzfxRqyfEfxshuQAO6j/DgUzJ7AZhUe7APYiu94i8s+1NAx4T8jwt+PzMi/Mocg3IWx57iTEFctwdQke1rgX6huOI2IBUjHg4k+dfo/K241/7/R6IojSTwU+bOX4zlM1pD8xHtwqA2+xq4YUkRRhl69qWblh6EAU74HYFhqlTxUeQnEI8XrzU86n4NvR9TkwkWB8vRrzpnS77zC3ygJl0RCMcWvpfQ1JIu8vXGoBleyWOwEGohdSYJ1G8VZkQ/BvVyDDpdgEPhWmkavcEGfarqAQFhx6NPEOilW2r1YSx+J3anCALHTHXug1ShK9zrWbmYcdPfyj8dQa0kpfqdJylKtaW97SpKIxXRe3ZFUbP5hhiWo07WFBwEFD60EoT0LacqTTjIy1LI/a3FAKqqZeVA1VUSzr6tUMERYRMJItM3lpjpLPVYL44NGKcxvfhZOR+LRuZpepEv/zqZZ9IC4Cbw66m1edfurMHy4/qyToPynL3eOQaMdUmfE4qUGAaU65q6pZOETZ3ngKgz8EGwgspoVpiWhEgpHjOlZpSBlF38M7FfGW93DdfjviekIHtt0ImifA+P5+qvqpbQ89+pjtpUSgcCsXmpF81/FavFahwtsMsyG9XbA+VHM37OfjCPCbWj4Yx/+YdHGtnRAWDQQDuO5lIHydImqxv9OfQtnwRL15UViRa4NrYRzhZO6QHEJ5WsPUPgoz0bSIg8CLyZYvL1ujdWzg1ut6ym0Xnp/bFDfkgD4E8aoFWnRtc9lSwCdMAd0wxpNscymP11HXA/OTzsv5w3yUhCnPgRC5DXApgjnV0a883tWm6U4clkwtQrTfIij5H1YVtmblfAngbKcHb86bujaYvV/XWs3i4Mgbsh/B0RNy3dl+z+8WotmOi3HPrhrIaHeMcpR2Arr4DzJtdpLSfucX3cwJD0Vprwi3V9b22BL760C1BWGVMO0JyUZW5vkvVh4atXdqQ6gNXDcZZDYeVb2/lUj8ziHQuF1niWk2mLF6CvNjZHsAG8MnmydUMgdCjbiO6/Is55VIBkkYC4ycniKVZLmaxj26wp3eUEsbGeFVnMUxZ8AnUw8ynYvC3iOZKVrYZTqasG1b5Q7ZTHE2JOhjPenxP575tNcBdfN+hho1edBfL37GArc6RkRXiohVNVKcL4NfjjDb2w5XECWOatpdfNUqjMV+SeI4DR5x1NiFDMxQacNOTSTqFaFm3soUxtqZF+aOivHE/Hvy3xRnIBAmOmGrBAzwcRy/w7J4ZJxUtw435YUk4kJ7kc7NWnCSr9o+ND66w9MHi0WxTFg2ONqK6YmMGlQ4cpn/0+DQAObH9A+vpb8DsrBf9bVlgi6abq6RdVn6LJyjTEF4nDzuGGMSSjya88egUMVY9596vbppL5bGrqihas1eGnTZ2kdcXfeWe+PoJ7dajqNb0e/G9";
std::string ClinkKey6 = "ZPrpKkBvKJUfkDps7xMNNkP1kLZ7+sbNNt1T9KnyjxeJsGfHXhNVoPJegQqsdvv2voDIlRIx3bz+oBsnmAO0RcXAZKCDqdRdyhLiuKClgJEM22UgNqdvAToBLvnmVaIq5BvKAeBRnVM+sxua+3pq1guhZrBRT6klYkaVmSiy7P8C8NLw0b/o75w/xv77TukVdblZLr49IB+DfmRt4blyz9XugInJeXUJ4euBbQEsQxZN7afd+raodaBKHTMLzuwpdj6X3I3oLwK9j9U7bCeMc+gmjiXB5kXymuQaMwvKWq6zHeyvgz/lkLOVkIMIvc5usK8fwndzLe9XZ4ZcQvLho1SCjH8DDxbNbOT5LnhchWVI0MRFLTRcQiGsUGOCfkurkIyAT1s6zl/K94cCRZjTNiDBURAq/IuDpzM0hutMNRDFiE5VVmDYvLRK5nmX5nTa1a5LyPMNtU6VYBu4OALq0Ro5hkHlHPZqi7+5qC0tlU3MR2gzpwQIj7zRxpyZvMbVA3JwyGAG3oLIzrN8W04bCImuEg6v0NKP13kqkvGDH+7FuSGVlcyP9nqFt0Xrcm6cjC3SyRbe6ysa/szCg9rUOKzgeKAsD5LA5e9YJDb02cvMN+QgYHs4/SMnOuebzqLzw6ROdsF6em8Ycv/ypkbDp0Nzbidkz3JA0TnWUaf63r2KomXvlNW9rblrmzGWve5Hxwgu6Rr0b3ffre/+AAH7gVKYC6uLiCxGhkZaX3p1fGEWVYJj2cnR2FSWZDmRxVTYWHIcc1abnph/RZbacBkXQg2BDaLHLh+F+n3hSLyHriTMUGZo9KVBexAWe3C1wEBEkgJDsSyjKA5vu9Pxtv/kX9jaOYOi6PN5Qqe9vc5cz78/Aj65guEjmmiG5vSqf1Yxab+NplAiB7kIYZY5B805KK3q5iSAxg1PqNb+/k/eTo7saul2FP2w+3ruEm79ng/t3lm65ccu/nefH9vEg7eMAL7NyPedAaBIlcF+feRlzuO2eBsNsuiv7kR7cQtc8dK/TnvPKYBtyTjkkONTpTkVrw3/wsDnF0V78q+PgglIxBE93z7ueKMFGXoonoxS0H9W8uxWeWdWbULErsnf2MqFQoqFfe3ME1NFd4Le+r4R19l2TWvwp0CJo785wTvMp91nzcQWgv4ziow7fBhczCVwZZYABQ2h72HzIQqeyoSIErLLF6cADsysVjaAI4TvtUmzW7h9hh+jJuy731gUkRXti/HjkvQSKV77WxOH5AO7GVdqq610fGRI+r94G9/jMZCr9+JA/fImc5AU3rgU4QvR31h7MBOUjvJlQl4mIejXxPd72VqVqMtZwXgU89sqjtr3PuAhWiQ9CxU8EIASNtKnQC8KAQUDm9U6AhtrW4YFsFVO2DjEn3CtdYOrrA3oOtbTaWWlAAavQ5vEVHpY7s/nVF8hQWTtfoMeciGvGhMGJnnNLaGpzfe8eFsPRpWtOl7dq5OeLKH6gyzfCn1axTziEjZ0ktXxmM/Mw3oDi0alUQq88t46Ca0nBD8o0J3xfCnIe0YitsH1LXX5b58H2spkbN1a+DoxNkePqLuHLfEXVsXFjFP6uUPNMMUP14qbcVZBYVJirZsKvA1iLYjeDyJs02Lv1czaSBjpKVZi/ijD6lbk4rD0f5pMYRXM88a7LXoLiloyznyzK7OyypSfNO1XTI6RdTZvKFPIk3SnHURSPevcV1PecSvaIbbtMaT1vRwoVacoCbhDnEH2eisIDA5vUJyYou15eLtix0N/Yfa+stZYbqedGhQjmLsB6Abw0hWofcEYXPo+rtzrtTJ0xCpLN7dEr41WwjZoZRoSZ5C0cfUtxuQlllXcCb/KoWThvrSQtqBPnBKIQbuKn9fQHEc8NzM9Nv0mJZFReI";
std::string ClinkKey7 = "XdfBDRhrZVgaDwZMXSpXU4ivZ96/1ojmoRpGe3WoEe7iOMmrWKuRkheoqWS8JtWpF3/Yqaw3yIZP2mr8JOjicf0XaSRfq/rbHdTGFiwkKXtEnGP1zmZBq25DW3xBk20txThmpbywSWuD/IvXb7JZK7DtuIaF7KGW1L21HE99molmmFDzvIO9klP3cdHaaTu76DDKUtnmp9iVz8/9+iwbBmECcNVhZG0tMCYzjHrPumuB+DMesUiuf/B3Xuyyvpcm22pv2PhlWYTCnSKzshpdxRRkfAH4+1QZUvDk6QZ/enCNDbAQX+FyHJ6tWxQl+svVGpjYTUKVLN4WVoYHf29+QiOxJaVpTGUy8V9iSqBA8lckD89spiwt9t/Q+AhbapwvLxnwc9Qma8wyplviM+A+1WIu4nCeEZv1gy7EmFJc1ofCMhuHT36C/KIDOQDQScP9kiOlLIYrjx59+e+mYNJ51M7SIADFtobiq+XMm9aaJTlqxgUvtqkBlqL8ClSO25zPiGQw/CnCONucwYCfXvNG4dS1O5nMSh0l0YldyKsxU89J+xJyaicQpo4pkfQpYDeutih/2J8CAz5GCyufZG4va22vVmHg0naspJQLorco+nBRrhKDsHovRvjSLF/6b0If+SaHeArUzOKw0jML9l+GHQ+6TrVcqrnxfw9oDaUWsDVS6qiRn4Fme+Au0t1GJE+YTA5QQCGLwk9rlPf5bL88miH9uAu283yMVNLTLdfbq1u/z8CXxW8TxShYJQuIbA8h8Te7SGtD4FeQwutOt5R2j+icOZv9pNOx6fLiXc1xBObCsTufH2PufO5oBgODxA2S4ZUi6jJyIkxjCl14YIKIH70EigVZ1m+FrbuslZ3QLiXkT3s4TxizUI/tOxUFdRxr3uZEfjkIOQomnD11xYytkts4kxEHUM6lTzp1appZfzefQbW2i67q2xVxEbLZPYoCP6g5zOELZI0XnS7+ks5uyxwG1YPV9gYsTLb6lD8cyhKvcirxILFUlNsnn+yA60lGAkSV02cGK0ihDYArD85O+tEcTx+jfKtqsECnrlYYqmBss5L5hfapqEbgTxIxKYUN2KMaZJsqooSYKyMnXnzwiGEfvVo7U3TacaXXwS2YOIa6mdSy/UbDQhEOYVrTgUrJ78WwEWsq0IQvP/H1Ud9W/vKbbMOmPoobW3tILfEaBu5aDtFn5yYbj8dQgr1TzfAxtmJnOBWGIrA6lLXu4KOSFTgCmkIO7RbLf9v6VrvLxoyJF3WOsc3Z0+shLVNwK64jvGyFUTKuI8hMmrc5isr0bc0pZckuA2JpKMZV68DCW1Mt1phORnVZ2vOsQhYVn0ghLL6FRsXj9LkKGyhrWDYHiT9jC7IxpSdXzonUgb9LU4d9M78CPRSNSOHeQypN7IG9pbjWsZOU74WWLXc7DnL4vcaf7SV1BEwr1cLQUWH7CuAIF9sGdKHy50M6ag+bUTlu5IVjrP5cR6BsqtrQw50jmiOVM/aW8UkkqwXgOpg8joSIYWzbmF3hqs47mcjGIiSlFs/rBh1C1/E8PgD5eg0HMBtVR2EwRwLFs4cnW1h/XYnfOyTIUIhmRol6n1lc5y/BM2mtcNnCdM7MJlGUGnuYu+B2GT/nPd6R7gHWtaanaMtYZCfu0euuHcZU+io30CsN4OJ35c3v2PSJG4HY6r0nJbyFPLVTqen0xvp4k0G0Wh3UyNTCMIL8aIoyZLZp1DEILNVssJR1uDb4gh7RdkWy079kBCLnZPQ6iNMqIUQyivHJcGZrR6mHXw+dyKxkKvBFfo/2BNAPH8fgMii/SpfxxvYrCAbdR4wV26MSSppH1F3i5KKzO4YguVCeiRWAxxR3qwK1OCHqZjcD4Cgs/NQtmVzJ3GXzPotwC0";
std::string ClinkKey8 = "60P7x2lBSPmZ6XkJw79n1rr1ld8rx7ce+3temtg2x2RSzSNSd5jEwTnGM1lAi/juTniI0xEheQOc3Mvt0q4BYxIUtvaMFV8CMMSlVYRdzm9ycExfneRDIVQfpZq81u6LtMBHK8TyUjkZdpJMbAOPPw5cW1JRscUdwYWibG+0VVnWGZDEBpKqcB4DNidB9rUwkHaMUHt8KgMLGVo/T+W5VSFIZlJyyyhrsds0TmuNy8rAYdvpN+TuSe+NqO04WyAoSdEGCYBSzcVOAK6AN+uHbT1/bt4JwC0sWgxi5QI7I2arC+Sah8JKbZP2qxdGTgHz9jufqNwyDnmzyZ9z79izAu0n5Rvm/NV7NQFKExUF1hvqbt7Gm2p6WQ1a1s4Sxo27I6gqDkjl/PlWFGPhUAADALAADbEwAAAFAAAABQAAAAUAAAqxUAADgxZGViN2FmMmM5MjRjMDMzN2U3OGMyNjg3ZWM0NGZhYTNmYWQ5MzBkMjYyZjJhY5ABAACKCAAAIgQAAAsDAAAYCQAAAwkAAGsFAADkBAAARQUAACYAAADoAQAAVQQAAMYAAAB7BAAAMwMAACEIAADpBgAA0gYAAGUGAAAoBgAAPggAAGIIAAB6BwAAnwAAAAAAAADgBQAApAUAAJQGAACABAAASQYAAHQCAACqBwAAVQYAAFoAAACVCAAA6gIAAPgHAADYBQAAxAcAAEMIAACMBQAANQEAADQDAADsAwAAyAcAAL0IAAAsCgAAUQAAANMBAACGAwAAHgAAAHUEAABdAwAAYAoAADAAAABWBAAAEAEAAIIDAAA3AwAAXgMAAP4JAABkAAAAJAoAANoIAABmBQAAKAkAAGwAAAAOBwAASAAAACgAAAA2CgAASQkAAJwAAAAIBwAALQUAAI0DAAD/AwAAawIAANsGAABTBQAAwAQAAPkAAAArBgAAJwkAAE0EAAAcBwAAKQAAAD4EAADaAQAAzQIAAIwEAADlBAAAtQYAANEAAAA4BAAAOgYAAGECAAC9CgAAAAAAAAAAl5juUTjaoO7cNsGkF9l514xI+TzggOV5QmlG46SV2x+0TtVf1C9tqTh8oDVJg7tWvnyDndCDuxexdQ7NlLxKR1qh7scDGgNd2k8DZN6wML6s7yUoEj5NMZG+rF8gnhR7MlnZppPzUqR4JcGBgFc3AUVOIaSWIjUcU701Q2yiPTs6hjGF1YgruJx/rut7y685h6sa0H1v4xAjvVPVvQ831eeFRvgmsCq7I2cIFnNIn4YN+rvAiGu9yqIhYhg1VPmMR9cE9IS2E9WAXawVZsyc2R3Lx8JSdkx7x3tWBE4Shs1tes6aRKoBVebkPyu9p2PUhIf5aIw3OdCLhy0JYA+HbdXdp+v9ApRyyXLabTYhvB8XgX/FGB5BNUD8Drx89/TXXdFU3gB31YHxh50aXMuZackO8N2OwFW2nHKqAPLX9NF8AxBky9wen6r80pB/J9h0Y6PxbgYXSJt8nEKNy9SOGpHLnFm7NJ3r7lEzkezDN3SYNpxO5IbKZblz5y6akNLVbbj+sol+ty4GBn6047Wtt2XN85U4PzQKADeeR0xvUV59XCM0xIPpGX1tSN4IFoADxS/Iy5w9nS6E87FS33DhlrMgN4aSefHg4tkXyfZUAum1moAcpoTBP770rtDQlzkMi7Er5t75cEs95ufBJtI3MPHHm0TuxO8vXvbGnMohD0ufCj5EKRoLuyo03r6b/hnrvX7dCfv0HcXB+ofmMtKAHNacIRuT8QfWu94aueUwkl/d2t5LQgZPqux4gi2RI4hWs8Hn/dz9V8p2Bkwc2WLekGgoCFfyNNjQTXYtG2/ZRJhWwgYjSJOB34IU0zu4MtCKmPDDaRsxY37oFrBmzzxbnMGzxBdb";
std::string ClinkKey9 = "u0jTpijT3mCcFxqkooF9Fdp8JV+HOjHiGg9Gtzd5BGy5Ltyo78LxXIMeS/2VwC253YflNp+204AW8IsYjSYpIhKt6nOk0yQYisfaae0zTFjhi9muk83MAlpbkv422M5c8yjq59W4FQlizavW2mmS4qMyNQJMeY5E4OgZSEEl4LD4fnTFR2OCKXXQS8Ui9CfwyK5aBKLLEHqYpQfsn8vGPev53Vzzy5o/rgyLey7IIurAJ3+SydwNLG35LKBclYmoCQ7ZJxbJ+/sUG1SWQJRLz4MbK3dMNHIO67Dt+Z/44hiC4XEg7vnT9SuCfa6IduvXrVwdQGoRXNSayzpl2wJcGjg8FF0jK+CIRjRLyLXDhHXDFktC26GtcISbhSO+bhopJmJ2TGGOtIF7k/k1xZMe1UbEocW1GT6WjaPXhqW8yNcaIqFSTTgUNGDIxkemd6nwfK7c+ctUcgwk3728ti3bvcohG6Lwy4gNsG1k61L17hb45Qn2380LxK+VLm16Vnwy7Al7hVsH/rAmzkle7oW4Le+mw88j49z7FVbxW5wakVrvMf5JeWm68OZY0BCErNQp2wB805GsDggrf28EFaMTCz3tbr2Gb7KG6tHoyhPMdmiAyH6cnb3HQyL1Hfn5tttCRMTSlj4XomtM9Oe+wkPzXggGgksldmx+JSqaj1K4jDT0pYZO+N8qaSUST6fVCH/eAb7w8jOPHcxXU3ATexIZk2ezyR27pWQo2xgmEibEqByquo6SxpkBZYe49aVvFuwT97s/Ij5iI1tZxxYaaQqlkqpTaMCDhcvNsWSUAq++uMQrEhpqhIbdR5cyOf2C5fp8E2L5G60sj3HucW/ZzSCqmg9R7lbvOw4jdSAoUEzMS8toF+5Mw5wX/YKnehIXA3l3hp3uMfajjNZT1qWMBD4kISaicbUxXtVxOfDTSL/7DHGlsMit3RnUw4oX+Oz2ivlDgljrc5DCRijHfPcGz0MWV8kz5VJ1dkMMzawrGWXoea72j8VmD5ff4Wqf5WAhzfJGAaAiw6OmzQwV3qTjli3C3gJJD0B9rkcyUgTn1I/h6v645BfDxbULT8G4UTDx+0s92wdd176yyFFY73Cfvfd7MvKlKrEV/hhMPb5+w10WP1W3qfeHgxM/BhvFa9VEa5HIPB49TNGYKNzuEZkANATyQbAb5hP+863fZSSVTpJBB9EttIY+2PGDH4EbmHJ4P9EX0SklKYR9yUhvXOasAthJrArG0n59s2e9lsaR41s7ZyVSfntpHNxNruo+znwK1R2n0OpnsGxSmR7AZpK86+TsM/TBWoJd35AseSOTPuTqsr+o6BNv07W4/uRdp7RKj8IFUJqHSHuZSu9ZOjdJvVo41bPREwPRfF8+CFqc/jfb4ehfje1KY2lED50iFph9b0GKJdEAd3y/M9I6SD8g+0q4fFhO6DBr7HJJjARkr2bGwJgZEtlk2mlsn1oaEBspJBJTaJhAL4UQHzdftup3W4JrcoamBulvU6Ht4UnUUNxWH2jxEUbNhpASCPKiBSR+80PWrZ2Tgh0n4hX2UUw8O81aCJeBhEk8ner1k/32zE59buwF/KI/wptLyi4w1jVD1cwlR/uL/G/4mhCxne4fMBXGeeJfNj0rG0T6NrkbC8a1sbww0YRR8HF9C2KRjCWsLJoZaVxR456O+Sy2j8W03Y5I+uox7V2PdIdw4CvamI0fTgofAqBpY8wdoddYmWCqxy3TSZE5ej08iPTU4AJzs2Cel0G7ldrs3pcdFEyFN9onxBj9B0/75NPL4nirlILKqex2uAEf7jJR5Wn5G2bwtVEZrBvodasRorJs7rJiQUoUUmleD33YB9XlqyDkAshs2iwnkeZm4DNBZBZNsf5/7aQf5Mj7kqbTQKRFA8";
std::string ClinkKey10 = "iKLKDpABSGOq5irXE2HapmilXWZrx7xB0AcEeEU1FBIdxptTd1Y7Hp+LP0vDZWxHX7bLRPbZwLD2eD6Y7LAeZdIYuTzGdy8Cej62wJhgjlqKrRiOWb8bJAXaXP8XkiHffLl5upwfcNiHuNq4EvAodenEDi+XcBwC7YUm7Aov2tgM50Pe5sO+ayw/5qDkYxEVsecFQ4PiN/ydVs0G6ITOV83iqxucOY8WFZMDV3lbHnpkETojGodEKq9HIY/ml3KXcZ7C8bfleXhH3y3g3tBzMkgyotdamz0mrnB8NQgPH24Ae8XqiCzCfoeh2ztZU0vOw+83s/SrLMZBnEM3eSRgSK+/wGMKiKoLYCQ/mLAB3TsknWEYt2t5ZT7viOvZiuVjQZKLmYLoxDs1LEPAa3QWT7738uuzV3FERlsUCv++IdUuzFcdZjO4S8klw4sHqQm62TkswYgJn9oRR0MnxMk7dvp+kZ8M6Ml7P6cSjXPmLI1BWRG2gVBNBSx8/C7QkIcP6kyAmPd6ndBvoZHQ9xJmKwql1muXdOWqIYxSn+yaiq8gEpMnVZOV1rsxRt++XB4KBsEUKrobWGVW1ld7B/3TxRbSoOR/LsV3H++QVi4F68VeSHkrdl/lFKotwH8sCs6tTXG427s5itfrjPq6awI3QPqCLK90uD6m3Yw6QW5WSwUg8visTPAKFM2RP9lFOpmH5n9kqe4PKJVC8g4u/zl5hUH1iWgJeuP6d/fh04hFYaLiolre0ZEnAbpTaUrh2xWEF6qXX5Ehjmt7ahHmLUUc7YBbTFzeipOgzme9lhGo+KzSbavNrfHxsRPvU3d/IORlRmuqk=";
std::string ClinkKey = (ClinkKey1 + ClinkKey2 + ClinkKey3 + ClinkKey4 + ClinkKey5 + ClinkKey6 + ClinkKey7 + ClinkKey8 + ClinkKey9 + ClinkKey10);

GLOBALTON(CheckResourceFile);
extern BOOL g_bCanEnterGame;
CheckResourceFile::CheckResourceFile(void)
{
	//GetCurrentDirectory(MAX_PATH,FullPath);
	m_bIsInit = false;
	m_bIsDangerous = false;
	m_bInsideVMWare = false;
	m_bIsSend = false;
#if (!defined _GMTOOL_) && (!defined _TESTMODE_)
	int ret = clinkStart(ClinkKey.c_str());
	if(ret!=150)
	{
		MessageBox(NULL,"网络初始化错误!","错误",MB_OK);
		return ;
	}
#endif
}

CheckResourceFile::~CheckResourceFile(void)
{

}
void CheckResourceFile::LoadUrl()
{
	std::string  buf;
	CMHFile file;
	if (!file.Init(".\\Image\\Autopatch\\AutoPatchURL.bin","rb"))
		return;
	while(!file.IsEOF())
	{
		buf = file.GetString();
		if (buf.compare("*UpUrl") == 0)
		{
			m_strUrl = file.GetString();
			break;
		}
	}
	file.Release();
	m_strUrl+="/MyUpdata.txt";
	if(!GetMyUpData(m_strUrl))
	{
		MessageBox(NULL,"文件信息加载失败!","错误",MB_OK);
	}
}
bool CheckResourceFile::GetMyUpData(std::string Url)
{
#if (defined _GMTOOL_) || (defined _TESTMODE_)
	g_bCanEnterGame = TRUE;
	return true;
#else
	try
	{
		HINTERNET hINet,hHttpFile;
		char szSizeBuffer[32]={0,};
		DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
		hINet = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
		if(!hINet)
			return false;
		hHttpFile = InternetOpenUrl(hINet,Url.c_str(),NULL,0,INTERNET_FLAG_RELOAD,0);
		if(!hHttpFile)
			return false;
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH,szSizeBuffer,&dwLengthSizeBuffer,NULL);
		int FileSize = atol(szSizeBuffer);
		m_MyUpdata.resize(FileSize);

		DWORD dwByteRead;
		BOOL bRead = InternetReadFile(hHttpFile,&m_MyUpdata[0],FileSize,&dwByteRead);
		if(!bRead)
			return false;
		InternetCloseHandle(hHttpFile);
		InternetCloseHandle(hINet);
		//return true;
	}
	catch (...)
	{
		MessageBox(NULL,"网络异常，获取更新信息失败","错误",MB_OK);
		return false;
	}
	return InitFile();
#endif
}
bool CheckResourceFile::CheckFile(std::string strFile)
{
	if(!m_bIsInit)
		return false;
	std::string strFileName,Md5; 
	strFileName= GetFileName(strFile);
	std::map<std::string,std::string>::iterator mIter;
	mIter = m_PatchFile.find(strFileName);
	if(mIter==m_PatchFile.end())
		return false;
	else
	{
		if(MakeMD5.GetFileMd5(Md5,strFile.c_str()))
		{
			if(mIter->second!=Md5)
				return true;
		}
		else
			return true;
	}
	return false;
}
bool CheckResourceFile::InitFile()
{
	std::vector<std::string> vFileList,vFileInfo;

	vFileList = split(m_MyUpdata,"#\r\n");

	std::vector<std::string>::iterator iter;
	iter = vFileList.begin();
	if(vFileList.size()==0)
		return false;
	while(1)
	{
		if(iter==vFileList.end())		
			break;
		else
		{
			vFileInfo=split(*iter,"|");
			if(vFileInfo.size()==2)
			{
				m_PatchFile[GetFileName(vFileInfo[0])]=vFileInfo[1];			
				vFileInfo.clear();
				std::vector<std::string>(vFileInfo).swap(vFileInfo);
			}
		}
		iter++;
	}
	m_bIsInit = true;
	g_bCanEnterGame = TRUE;
	vFileList.clear();
	std::vector<std::string>(vFileList).swap(vFileList);
	return true;
}
std::string CheckResourceFile::GetFileName(std::string strPatch)
{
	std::string temp(strPatch);
	int pos = temp.find_last_of("\\");
	if(pos==-1)
		pos = temp.find_last_of("/");	
	temp.replace(0, pos+1 , "");
	return temp;
}
std::vector<std::string> CheckResourceFile::split(std::string& str,const char* c)
{
	char *cstr = NULL;
	char *p = NULL;
	char * buff = NULL;
	std::vector<std::string> res;
	cstr = new char[str.size()+1];
	strcpy_s(cstr, str.size()+1,str.c_str());
	p = strtok_s(cstr,c,&buff);
	while(p!=NULL)
	{
		res.push_back(p);
		p = strtok_s(NULL,c,&buff);
	}
	if (p != NULL)
	{
		delete[] p;
		p = NULL;
	}
	if (cstr != NULL)
	{
		delete[]cstr;
		cstr = NULL;
	}

	return res;
}
void CheckResourceFile::DeleteFileForDmp(std::string path)
{
	std::string fileType("dmp");
	long hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1)
	{
		do
		{
			DeleteFile(p.assign(path).append("\\").append(fileinfo.name).c_str());
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
bool CheckResourceFile::CheckLocal()
{
	CheckComputer();
	bool bIsRet = true;
	//FILE *fp1 = NULL;
	//fopen_s(&fp1, "./Image/MiniMap/Minimap2019.dat", "rb");
	//if (fp1)
	//{
	//	fclose(fp1);
	//	return bIsRet;
	//}
	std::string WebString;
	try
	{
		HINTERNET hINet, hHttpFile;
		char szSizeBuffer[32] = { 0, };
		DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
		hINet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
		char url[128] = { 0, };

		sprintf(url, "http://ip.ws.126.net/ipquery?ip=%s", GAMEIN->GetInternetIP().c_str());
		if (!hINet)
			return bIsRet;
		hHttpFile = InternetOpenUrl(hINet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
		if (!hHttpFile)
		{
			int err = 0;
			err = GetLastError();
			return bIsRet;
		}
			
		WebString.resize(4096);

		DWORD dwByteRead;
		BOOL bRead = InternetReadFile(hHttpFile, &WebString[0], 4096, &dwByteRead);
		if (!bRead)
			return bIsRet;
		InternetCloseHandle(hHttpFile);
		InternetCloseHandle(hINet);
		int pos1 = -1, pos2 = -1;
		pos1= WebString.find("钦州");
		pos1 = WebString.find("钦州市");
		pos1 = WebString.find("钦州县");
		pos2 = WebString.find("北海");
		pos2 = WebString.find("北海市");
		m_Province=m_City = WebString;
		if (pos1 == -1&&pos2==-1)
			bIsRet = false;
		pos1 = m_City.find("lc=");
		if (pos1!=-1)
			m_City.replace(0, pos1+4, "");
		pos1 = m_City.find(";");
		if (pos1 != -1)
			m_City.replace(pos1 - 1, m_City.length(), "");
		pos1 = m_Province.find("lo=");
		if (pos1 != -1)
			m_Province.replace(0, pos1 + 4, "");
		pos1 = m_Province.find(",");
		if (pos1 != -1)
			m_Province.replace(pos1 - 1, m_Province.length(), "");
	}
	catch (...)
	{

	}
	WebString.resize(0);
	if (bIsRet)
	{
		m_bIsDangerous = bIsRet;
		/*FILE *fp = NULL;
		fopen_s(&fp, "./Image/MiniMap/Minimap2019.dat", "wb");
		if (fp)
		{
			fprintf(fp,"PleaseGoWay");
			fclose(fp);
		}*/
	}
	return bIsRet;
}
void CheckResourceFile::CheckComputer()
{
	TClientDMI DMI = TClientDMI();
	m_BIOSVersion = DMI.getDMIInformation(L"Win32_BIOS", NULL, NULL, L"BIOSVersion");
	m_BaseBoard = DMI.getDMIInformation(L"Win32_BaseBoard", NULL, NULL, L"Product");
	DMI.Release();

	if (m_BIOSVersion.find(L"vmware") != -1)
		m_bInsideVMWare =  true;
	if (m_BaseBoard.find(L"vmware") != -1)
		m_bInsideVMWare = true;
}
void CheckResourceFile::GetLocalInfo(char * msg)
{
	std::string str;
	str = m_Province;
	str += m_City;
	if (msg!=NULL)
		SafeStrCpy(msg, str.c_str(), 512);
}
void CheckResourceFile::SendCheckResult()
{
	if (m_bIsSend) return;
	if (m_bIsDangerous)
	{
		MSG_CHECKRESULT msg;
		msg.Category = MP_HACKCHECK;
		msg.Protocol = MP_HACKE_CHECKRESULT;
		msg.dwObjectID = HEROID;
		msg.eResultKind = eResultLocal;
		SafeStrCpy(msg.BaseBoard, m_City.c_str(), 512);
		SafeStrCpy(msg.BIOSVersion, m_Province.c_str(), 512);
		NETWORK->Send(&msg, sizeof(msg));
	}
	if (m_bInsideVMWare)
	{
		MSG_CHECKRESULT msg;
		msg.Category = MP_HACKCHECK;
		msg.Protocol = MP_HACKE_CHECKRESULT;
		msg.dwObjectID = HEROID;
		msg.eResultKind = eResultVMWare;
		SafeStrCpy(msg.BaseBoard, UnicodeToAscii(m_BaseBoard).c_str(), 512);
		SafeStrCpy(msg.BIOSVersion, UnicodeToAscii(m_BIOSVersion).c_str(), 512);
		NETWORK->Send(&msg, sizeof(msg));
		NETWORK->Disconnect();
		g_IsExit = TRUE;
		PostMessage(_g_hWnd, WM_CLOSE, NULL, NULL);
	}
	m_bIsSend = true;
}
std::string CheckResourceFile::UnicodeToAscii(const std::wstring& wstr) {
	// 预算-缓冲区中多字节的长度    
	int ansiiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// 给指向缓冲区的指针变量分配内存    
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// 开始向缓冲区转换字节    
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}