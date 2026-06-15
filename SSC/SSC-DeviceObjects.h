/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
* \addtogroup SSC-Device SSC-Device
* @{
*/

/**
\file SSC-DeviceObjects
\author ET9300Utilities.ApplicationHandler (Version 1.6.3.0) | EthercatSSC@beckhoff.com

\brief SSC-Device specific objects<br>
\brief NOTE : This file will be overwritten if a new object dictionary is generated!<br>
*/

#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
#define PROTO
#else
#define PROTO extern
#endif
/******************************************************************************
*                    Object 0x1600 : DO Unit2
******************************************************************************/
/**
* \addtogroup 0x1600 0x1600 | DO Unit2
* @{
* \brief Object 0x1600 (DO Unit2) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
* SubIndex 65<br>
* SubIndex 66<br>
* SubIndex 67<br>
* SubIndex 68<br>
* SubIndex 69<br>
* SubIndex 70<br>
* SubIndex 71<br>
* SubIndex 72<br>
* SubIndex 73<br>
* SubIndex 74<br>
* SubIndex 75<br>
* SubIndex 76<br>
* SubIndex 77<br>
* SubIndex 78<br>
* SubIndex 79<br>
* SubIndex 80<br>
* SubIndex 81<br>
* SubIndex 82<br>
* SubIndex 83<br>
* SubIndex 84<br>
* SubIndex 85<br>
* SubIndex 86<br>
* SubIndex 87<br>
* SubIndex 88<br>
* SubIndex 89<br>
* SubIndex 90<br>
* SubIndex 91<br>
* SubIndex 92<br>
* SubIndex 93<br>
* SubIndex 94<br>
* SubIndex 95<br>
* SubIndex 96<br>
* SubIndex 97<br>
* SubIndex 98<br>
* SubIndex 99<br>
* SubIndex 100<br>
* SubIndex 101<br>
* SubIndex 102<br>
* SubIndex 103<br>
* SubIndex 104<br>
* SubIndex 105<br>
* SubIndex 106<br>
* SubIndex 107<br>
* SubIndex 108<br>
* SubIndex 109<br>
* SubIndex 110<br>
* SubIndex 111<br>
* SubIndex 112<br>
* SubIndex 113<br>
* SubIndex 114<br>
* SubIndex 115<br>
* SubIndex 116<br>
* SubIndex 117<br>
* SubIndex 118<br>
* SubIndex 119<br>
* SubIndex 120<br>
* SubIndex 121<br>
* SubIndex 122<br>
* SubIndex 123<br>
* SubIndex 124<br>
* SubIndex 125<br>
* SubIndex 126<br>
* SubIndex 127<br>
* SubIndex 128<br>
* SubIndex 129<br>
* SubIndex 130<br>
* SubIndex 131<br>
* SubIndex 132<br>
* SubIndex 133<br>
* SubIndex 134<br>
* SubIndex 135<br>
* SubIndex 136<br>
* SubIndex 137<br>
* SubIndex 138<br>
* SubIndex 139<br>
* SubIndex 140<br>
* SubIndex 141<br>
* SubIndex 142<br>
* SubIndex 143<br>
* SubIndex 144<br>
* SubIndex 145<br>
* SubIndex 146<br>
* SubIndex 147<br>
* SubIndex 148<br>
* SubIndex 149<br>
* SubIndex 150<br>
* SubIndex 151<br>
* SubIndex 152<br>
* SubIndex 153<br>
* SubIndex 154<br>
* SubIndex 155<br>
* SubIndex 156<br>
* SubIndex 157<br>
* SubIndex 158<br>
* SubIndex 159<br>
* SubIndex 160<br>
* SubIndex 161<br>
* SubIndex 162<br>
* SubIndex 163<br>
* SubIndex 164<br>
* SubIndex 165<br>
* SubIndex 166<br>
* SubIndex 167<br>
* SubIndex 168<br>
* SubIndex 169<br>
* SubIndex 170<br>
* SubIndex 171<br>
* SubIndex 172<br>
* SubIndex 173<br>
* SubIndex 174<br>
* SubIndex 175<br>
* SubIndex 176<br>
* SubIndex 177<br>
* SubIndex 178<br>
* SubIndex 179<br>
* SubIndex 180<br>
* SubIndex 181<br>
* SubIndex 182<br>
* SubIndex 183<br>
* SubIndex 184<br>
* SubIndex 185<br>
* SubIndex 186<br>
* SubIndex 187<br>
* SubIndex 188<br>
* SubIndex 189<br>
* SubIndex 190<br>
* SubIndex 191<br>
* SubIndex 192<br>
* SubIndex 193<br>
* SubIndex 194<br>
* SubIndex 195<br>
* SubIndex 196<br>
* SubIndex 197<br>
* SubIndex 198<br>
* SubIndex 199<br>
* SubIndex 200<br>
* SubIndex 201<br>
* SubIndex 202<br>
* SubIndex 203<br>
* SubIndex 204<br>
* SubIndex 205<br>
* SubIndex 206<br>
* SubIndex 207<br>
* SubIndex 208<br>
* SubIndex 209<br>
* SubIndex 210<br>
* SubIndex 211<br>
* SubIndex 212<br>
* SubIndex 213<br>
* SubIndex 214<br>
* SubIndex 215<br>
* SubIndex 216<br>
* SubIndex 217<br>
* SubIndex 218<br>
* SubIndex 219<br>
* SubIndex 220<br>
* SubIndex 221<br>
* SubIndex 222<br>
* SubIndex 223<br>
* SubIndex 224<br>
* SubIndex 225<br>
* SubIndex 226<br>
* SubIndex 227<br>
* SubIndex 228<br>
* SubIndex 229<br>
* SubIndex 230<br>
* SubIndex 231<br>
* SubIndex 232<br>
* SubIndex 233<br>
* SubIndex 234<br>
* SubIndex 235<br>
* SubIndex 236<br>
* SubIndex 237<br>
* SubIndex 238<br>
* SubIndex 239<br>
* SubIndex 240<br>
* SubIndex 241<br>
* SubIndex 242<br>
* SubIndex 243<br>
* SubIndex 244<br>
* SubIndex 245<br>
* SubIndex 246<br>
* SubIndex 247<br>
* SubIndex 248<br>
* SubIndex 249<br>
* SubIndex 250<br>
* SubIndex 251<br>
* SubIndex 252<br>
* SubIndex 253<br>
* SubIndex 254<br>
* SubIndex 255<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1600[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex64 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex65 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex66 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex67 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex68 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex69 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex70 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex71 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex72 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex73 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex74 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex75 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex76 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex77 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex78 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex79 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex80 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex81 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex82 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex83 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex84 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex85 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex86 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex87 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex88 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex89 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex90 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex91 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex92 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex93 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex94 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex95 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex96 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex97 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex98 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex99 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex100 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex101 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex102 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex103 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex104 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex105 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex106 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex107 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex108 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex109 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex110 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex111 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex112 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex113 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex114 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex115 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex116 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex117 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex118 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex119 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex120 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex121 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex122 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex123 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex124 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex125 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex126 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex127 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex128 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex129 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex130 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex131 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex132 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex133 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex134 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex135 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex136 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex137 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex138 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex139 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex140 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex141 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex142 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex143 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex144 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex145 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex146 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex147 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex148 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex149 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex150 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex151 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex152 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex153 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex154 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex155 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex156 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex157 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex158 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex159 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex160 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex161 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex162 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex163 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex164 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex165 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex166 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex167 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex168 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex169 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex170 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex171 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex172 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex173 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex174 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex175 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex176 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex177 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex178 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex179 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex180 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex181 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex182 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex183 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex184 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex185 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex186 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex187 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex188 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex189 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex190 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex191 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex192 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex193 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex194 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex195 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex196 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex197 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex198 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex199 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex200 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex201 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex202 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex203 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex204 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex205 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex206 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex207 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex208 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex209 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex210 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex211 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex212 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex213 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex214 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex215 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex216 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex217 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex218 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex219 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex220 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex221 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex222 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex223 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex224 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex225 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex226 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex227 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex228 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex229 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex230 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex231 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex232 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex233 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex234 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex235 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex236 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex237 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex238 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex239 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex240 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex241 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex242 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex243 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex244 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex245 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex246 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex247 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex248 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex249 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex250 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex251 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex252 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex253 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex254 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex255 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1600[] = "DO Unit2\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000"
"SubIndex 014\000"
"SubIndex 015\000"
"SubIndex 016\000"
"SubIndex 017\000"
"SubIndex 018\000"
"SubIndex 019\000"
"SubIndex 020\000"
"SubIndex 021\000"
"SubIndex 022\000"
"SubIndex 023\000"
"SubIndex 024\000"
"SubIndex 025\000"
"SubIndex 026\000"
"SubIndex 027\000"
"SubIndex 028\000"
"SubIndex 029\000"
"SubIndex 030\000"
"SubIndex 031\000"
"SubIndex 032\000"
"SubIndex 033\000"
"SubIndex 034\000"
"SubIndex 035\000"
"SubIndex 036\000"
"SubIndex 037\000"
"SubIndex 038\000"
"SubIndex 039\000"
"SubIndex 040\000"
"SubIndex 041\000"
"SubIndex 042\000"
"SubIndex 043\000"
"SubIndex 044\000"
"SubIndex 045\000"
"SubIndex 046\000"
"SubIndex 047\000"
"SubIndex 048\000"
"SubIndex 049\000"
"SubIndex 050\000"
"SubIndex 051\000"
"SubIndex 052\000"
"SubIndex 053\000"
"SubIndex 054\000"
"SubIndex 055\000"
"SubIndex 056\000"
"SubIndex 057\000"
"SubIndex 058\000"
"SubIndex 059\000"
"SubIndex 060\000"
"SubIndex 061\000"
"SubIndex 062\000"
"SubIndex 063\000"
"SubIndex 064\000"
"SubIndex 065\000"
"SubIndex 066\000"
"SubIndex 067\000"
"SubIndex 068\000"
"SubIndex 069\000"
"SubIndex 070\000"
"SubIndex 071\000"
"SubIndex 072\000"
"SubIndex 073\000"
"SubIndex 074\000"
"SubIndex 075\000"
"SubIndex 076\000"
"SubIndex 077\000"
"SubIndex 078\000"
"SubIndex 079\000"
"SubIndex 080\000"
"SubIndex 081\000"
"SubIndex 082\000"
"SubIndex 083\000"
"SubIndex 084\000"
"SubIndex 085\000"
"SubIndex 086\000"
"SubIndex 087\000"
"SubIndex 088\000"
"SubIndex 089\000"
"SubIndex 090\000"
"SubIndex 091\000"
"SubIndex 092\000"
"SubIndex 093\000"
"SubIndex 094\000"
"SubIndex 095\000"
"SubIndex 096\000"
"SubIndex 097\000"
"SubIndex 098\000"
"SubIndex 099\000"
"SubIndex 100\000"
"SubIndex 101\000"
"SubIndex 102\000"
"SubIndex 103\000"
"SubIndex 104\000"
"SubIndex 105\000"
"SubIndex 106\000"
"SubIndex 107\000"
"SubIndex 108\000"
"SubIndex 109\000"
"SubIndex 110\000"
"SubIndex 111\000"
"SubIndex 112\000"
"SubIndex 113\000"
"SubIndex 114\000"
"SubIndex 115\000"
"SubIndex 116\000"
"SubIndex 117\000"
"SubIndex 118\000"
"SubIndex 119\000"
"SubIndex 120\000"
"SubIndex 121\000"
"SubIndex 122\000"
"SubIndex 123\000"
"SubIndex 124\000"
"SubIndex 125\000"
"SubIndex 126\000"
"SubIndex 127\000"
"SubIndex 128\000"
"SubIndex 129\000"
"SubIndex 130\000"
"SubIndex 131\000"
"SubIndex 132\000"
"SubIndex 133\000"
"SubIndex 134\000"
"SubIndex 135\000"
"SubIndex 136\000"
"SubIndex 137\000"
"SubIndex 138\000"
"SubIndex 139\000"
"SubIndex 140\000"
"SubIndex 141\000"
"SubIndex 142\000"
"SubIndex 143\000"
"SubIndex 144\000"
"SubIndex 145\000"
"SubIndex 146\000"
"SubIndex 147\000"
"SubIndex 148\000"
"SubIndex 149\000"
"SubIndex 150\000"
"SubIndex 151\000"
"SubIndex 152\000"
"SubIndex 153\000"
"SubIndex 154\000"
"SubIndex 155\000"
"SubIndex 156\000"
"SubIndex 157\000"
"SubIndex 158\000"
"SubIndex 159\000"
"SubIndex 160\000"
"SubIndex 161\000"
"SubIndex 162\000"
"SubIndex 163\000"
"SubIndex 164\000"
"SubIndex 165\000"
"SubIndex 166\000"
"SubIndex 167\000"
"SubIndex 168\000"
"SubIndex 169\000"
"SubIndex 170\000"
"SubIndex 171\000"
"SubIndex 172\000"
"SubIndex 173\000"
"SubIndex 174\000"
"SubIndex 175\000"
"SubIndex 176\000"
"SubIndex 177\000"
"SubIndex 178\000"
"SubIndex 179\000"
"SubIndex 180\000"
"SubIndex 181\000"
"SubIndex 182\000"
"SubIndex 183\000"
"SubIndex 184\000"
"SubIndex 185\000"
"SubIndex 186\000"
"SubIndex 187\000"
"SubIndex 188\000"
"SubIndex 189\000"
"SubIndex 190\000"
"SubIndex 191\000"
"SubIndex 192\000"
"SubIndex 193\000"
"SubIndex 194\000"
"SubIndex 195\000"
"SubIndex 196\000"
"SubIndex 197\000"
"SubIndex 198\000"
"SubIndex 199\000"
"SubIndex 200\000"
"SubIndex 201\000"
"SubIndex 202\000"
"SubIndex 203\000"
"SubIndex 204\000"
"SubIndex 205\000"
"SubIndex 206\000"
"SubIndex 207\000"
"SubIndex 208\000"
"SubIndex 209\000"
"SubIndex 210\000"
"SubIndex 211\000"
"SubIndex 212\000"
"SubIndex 213\000"
"SubIndex 214\000"
"SubIndex 215\000"
"SubIndex 216\000"
"SubIndex 217\000"
"SubIndex 218\000"
"SubIndex 219\000"
"SubIndex 220\000"
"SubIndex 221\000"
"SubIndex 222\000"
"SubIndex 223\000"
"SubIndex 224\000"
"SubIndex 225\000"
"SubIndex 226\000"
"SubIndex 227\000"
"SubIndex 228\000"
"SubIndex 229\000"
"SubIndex 230\000"
"SubIndex 231\000"
"SubIndex 232\000"
"SubIndex 233\000"
"SubIndex 234\000"
"SubIndex 235\000"
"SubIndex 236\000"
"SubIndex 237\000"
"SubIndex 238\000"
"SubIndex 239\000"
"SubIndex 240\000"
"SubIndex 241\000"
"SubIndex 242\000"
"SubIndex 243\000"
"SubIndex 244\000"
"SubIndex 245\000"
"SubIndex 246\000"
"SubIndex 247\000"
"SubIndex 248\000"
"SubIndex 249\000"
"SubIndex 250\000"
"SubIndex 251\000"
"SubIndex 252\000"
"SubIndex 253\000"
"SubIndex 254\000"
"SubIndex 255\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
UINT32 SI65; /* Subindex65 -  */
UINT32 SI66; /* Subindex66 -  */
UINT32 SI67; /* Subindex67 -  */
UINT32 SI68; /* Subindex68 -  */
UINT32 SI69; /* Subindex69 -  */
UINT32 SI70; /* Subindex70 -  */
UINT32 SI71; /* Subindex71 -  */
UINT32 SI72; /* Subindex72 -  */
UINT32 SI73; /* Subindex73 -  */
UINT32 SI74; /* Subindex74 -  */
UINT32 SI75; /* Subindex75 -  */
UINT32 SI76; /* Subindex76 -  */
UINT32 SI77; /* Subindex77 -  */
UINT32 SI78; /* Subindex78 -  */
UINT32 SI79; /* Subindex79 -  */
UINT32 SI80; /* Subindex80 -  */
UINT32 SI81; /* Subindex81 -  */
UINT32 SI82; /* Subindex82 -  */
UINT32 SI83; /* Subindex83 -  */
UINT32 SI84; /* Subindex84 -  */
UINT32 SI85; /* Subindex85 -  */
UINT32 SI86; /* Subindex86 -  */
UINT32 SI87; /* Subindex87 -  */
UINT32 SI88; /* Subindex88 -  */
UINT32 SI89; /* Subindex89 -  */
UINT32 SI90; /* Subindex90 -  */
UINT32 SI91; /* Subindex91 -  */
UINT32 SI92; /* Subindex92 -  */
UINT32 SI93; /* Subindex93 -  */
UINT32 SI94; /* Subindex94 -  */
UINT32 SI95; /* Subindex95 -  */
UINT32 SI96; /* Subindex96 -  */
UINT32 SI97; /* Subindex97 -  */
UINT32 SI98; /* Subindex98 -  */
UINT32 SI99; /* Subindex99 -  */
UINT32 SI100; /* Subindex100 -  */
UINT32 SI101; /* Subindex101 -  */
UINT32 SI102; /* Subindex102 -  */
UINT32 SI103; /* Subindex103 -  */
UINT32 SI104; /* Subindex104 -  */
UINT32 SI105; /* Subindex105 -  */
UINT32 SI106; /* Subindex106 -  */
UINT32 SI107; /* Subindex107 -  */
UINT32 SI108; /* Subindex108 -  */
UINT32 SI109; /* Subindex109 -  */
UINT32 SI110; /* Subindex110 -  */
UINT32 SI111; /* Subindex111 -  */
UINT32 SI112; /* Subindex112 -  */
UINT32 SI113; /* Subindex113 -  */
UINT32 SI114; /* Subindex114 -  */
UINT32 SI115; /* Subindex115 -  */
UINT32 SI116; /* Subindex116 -  */
UINT32 SI117; /* Subindex117 -  */
UINT32 SI118; /* Subindex118 -  */
UINT32 SI119; /* Subindex119 -  */
UINT32 SI120; /* Subindex120 -  */
UINT32 SI121; /* Subindex121 -  */
UINT32 SI122; /* Subindex122 -  */
UINT32 SI123; /* Subindex123 -  */
UINT32 SI124; /* Subindex124 -  */
UINT32 SI125; /* Subindex125 -  */
UINT32 SI126; /* Subindex126 -  */
UINT32 SI127; /* Subindex127 -  */
UINT32 SI128; /* Subindex128 -  */
UINT32 SI129; /* Subindex129 -  */
UINT32 SI130; /* Subindex130 -  */
UINT32 SI131; /* Subindex131 -  */
UINT32 SI132; /* Subindex132 -  */
UINT32 SI133; /* Subindex133 -  */
UINT32 SI134; /* Subindex134 -  */
UINT32 SI135; /* Subindex135 -  */
UINT32 SI136; /* Subindex136 -  */
UINT32 SI137; /* Subindex137 -  */
UINT32 SI138; /* Subindex138 -  */
UINT32 SI139; /* Subindex139 -  */
UINT32 SI140; /* Subindex140 -  */
UINT32 SI141; /* Subindex141 -  */
UINT32 SI142; /* Subindex142 -  */
UINT32 SI143; /* Subindex143 -  */
UINT32 SI144; /* Subindex144 -  */
UINT32 SI145; /* Subindex145 -  */
UINT32 SI146; /* Subindex146 -  */
UINT32 SI147; /* Subindex147 -  */
UINT32 SI148; /* Subindex148 -  */
UINT32 SI149; /* Subindex149 -  */
UINT32 SI150; /* Subindex150 -  */
UINT32 SI151; /* Subindex151 -  */
UINT32 SI152; /* Subindex152 -  */
UINT32 SI153; /* Subindex153 -  */
UINT32 SI154; /* Subindex154 -  */
UINT32 SI155; /* Subindex155 -  */
UINT32 SI156; /* Subindex156 -  */
UINT32 SI157; /* Subindex157 -  */
UINT32 SI158; /* Subindex158 -  */
UINT32 SI159; /* Subindex159 -  */
UINT32 SI160; /* Subindex160 -  */
UINT32 SI161; /* Subindex161 -  */
UINT32 SI162; /* Subindex162 -  */
UINT32 SI163; /* Subindex163 -  */
UINT32 SI164; /* Subindex164 -  */
UINT32 SI165; /* Subindex165 -  */
UINT32 SI166; /* Subindex166 -  */
UINT32 SI167; /* Subindex167 -  */
UINT32 SI168; /* Subindex168 -  */
UINT32 SI169; /* Subindex169 -  */
UINT32 SI170; /* Subindex170 -  */
UINT32 SI171; /* Subindex171 -  */
UINT32 SI172; /* Subindex172 -  */
UINT32 SI173; /* Subindex173 -  */
UINT32 SI174; /* Subindex174 -  */
UINT32 SI175; /* Subindex175 -  */
UINT32 SI176; /* Subindex176 -  */
UINT32 SI177; /* Subindex177 -  */
UINT32 SI178; /* Subindex178 -  */
UINT32 SI179; /* Subindex179 -  */
UINT32 SI180; /* Subindex180 -  */
UINT32 SI181; /* Subindex181 -  */
UINT32 SI182; /* Subindex182 -  */
UINT32 SI183; /* Subindex183 -  */
UINT32 SI184; /* Subindex184 -  */
UINT32 SI185; /* Subindex185 -  */
UINT32 SI186; /* Subindex186 -  */
UINT32 SI187; /* Subindex187 -  */
UINT32 SI188; /* Subindex188 -  */
UINT32 SI189; /* Subindex189 -  */
UINT32 SI190; /* Subindex190 -  */
UINT32 SI191; /* Subindex191 -  */
UINT32 SI192; /* Subindex192 -  */
UINT32 SI193; /* Subindex193 -  */
UINT32 SI194; /* Subindex194 -  */
UINT32 SI195; /* Subindex195 -  */
UINT32 SI196; /* Subindex196 -  */
UINT32 SI197; /* Subindex197 -  */
UINT32 SI198; /* Subindex198 -  */
UINT32 SI199; /* Subindex199 -  */
UINT32 SI200; /* Subindex200 -  */
UINT32 SI201; /* Subindex201 -  */
UINT32 SI202; /* Subindex202 -  */
UINT32 SI203; /* Subindex203 -  */
UINT32 SI204; /* Subindex204 -  */
UINT32 SI205; /* Subindex205 -  */
UINT32 SI206; /* Subindex206 -  */
UINT32 SI207; /* Subindex207 -  */
UINT32 SI208; /* Subindex208 -  */
UINT32 SI209; /* Subindex209 -  */
UINT32 SI210; /* Subindex210 -  */
UINT32 SI211; /* Subindex211 -  */
UINT32 SI212; /* Subindex212 -  */
UINT32 SI213; /* Subindex213 -  */
UINT32 SI214; /* Subindex214 -  */
UINT32 SI215; /* Subindex215 -  */
UINT32 SI216; /* Subindex216 -  */
UINT32 SI217; /* Subindex217 -  */
UINT32 SI218; /* Subindex218 -  */
UINT32 SI219; /* Subindex219 -  */
UINT32 SI220; /* Subindex220 -  */
UINT32 SI221; /* Subindex221 -  */
UINT32 SI222; /* Subindex222 -  */
UINT32 SI223; /* Subindex223 -  */
UINT32 SI224; /* Subindex224 -  */
UINT32 SI225; /* Subindex225 -  */
UINT32 SI226; /* Subindex226 -  */
UINT32 SI227; /* Subindex227 -  */
UINT32 SI228; /* Subindex228 -  */
UINT32 SI229; /* Subindex229 -  */
UINT32 SI230; /* Subindex230 -  */
UINT32 SI231; /* Subindex231 -  */
UINT32 SI232; /* Subindex232 -  */
UINT32 SI233; /* Subindex233 -  */
UINT32 SI234; /* Subindex234 -  */
UINT32 SI235; /* Subindex235 -  */
UINT32 SI236; /* Subindex236 -  */
UINT32 SI237; /* Subindex237 -  */
UINT32 SI238; /* Subindex238 -  */
UINT32 SI239; /* Subindex239 -  */
UINT32 SI240; /* Subindex240 -  */
UINT32 SI241; /* Subindex241 -  */
UINT32 SI242; /* Subindex242 -  */
UINT32 SI243; /* Subindex243 -  */
UINT32 SI244; /* Subindex244 -  */
UINT32 SI245; /* Subindex245 -  */
UINT32 SI246; /* Subindex246 -  */
UINT32 SI247; /* Subindex247 -  */
UINT32 SI248; /* Subindex248 -  */
UINT32 SI249; /* Subindex249 -  */
UINT32 SI250; /* Subindex250 -  */
UINT32 SI251; /* Subindex251 -  */
UINT32 SI252; /* Subindex252 -  */
UINT32 SI253; /* Subindex253 -  */
UINT32 SI254; /* Subindex254 -  */
UINT32 SI255; /* Subindex255 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1600;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1600 DOUnit20x1600
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={255,0x70000110,0x70000210,0x70000310,0x70000410,0x70000510,0x70000610,0x70000710,0x70000810,0x70000910,0x70000A10,0x70000B10,0x70000C10,0x70000D10,0x70000E10,0x70000F10,0x70001010,0x70001110,0x70001210,0x70001310,0x70001410,0x70001510,0x70001610,0x70001710,0x70001810,0x70001910,0x70001A10,0x70001B10,0x70001C10,0x70001D10,0x70001E10,0x70001F10,0x70002010,0x70002110,0x70002210,0x70002310,0x70002410,0x70002510,0x70002610,0x70002710,0x70002810,0x70002910,0x70002A10,0x70002B10,0x70002C10,0x70002D10,0x70002E10,0x70002F10,0x70003010,0x70003110,0x70003210,0x70003310,0x70003410,0x70003510,0x70003610,0x70003710,0x70003810,0x70003910,0x70003A10,0x70003B10,0x70003C10,0x70003D10,0x70003E10,0x70003F10,0x70004010,0x70004110,0x70004210,0x70004310,0x70004410,0x70004510,0x70004610,0x70004710,0x70004810,0x70004910,0x70004A10,0x70004B10,0x70004C10,0x70004D10,0x70004E10,0x70004F10,0x70005010,0x70005110,0x70005210,0x70005310,0x70005410,0x70005510,0x70005610,0x70005710,0x70005810,0x70005910,0x70005A10,0x70005B10,0x70005C10,0x70005D10,0x70005E10,0x70005F10,0x70006010,0x70006110,0x70006210,0x70006310,0x70006410,0x70006510,0x70006610,0x70006710,0x70006810,0x70006910,0x70006A10,0x70006B10,0x70006C10,0x70006D10,0x70006E10,0x70006F10,0x70007010,0x70007110,0x70007210,0x70007310,0x70007410,0x70007510,0x70007610,0x70007710,0x70007810,0x70007910,0x70007A10,0x70007B10,0x70007C10,0x70007D10,0x70007E10,0x70007F10,0x70008010,0x70008110,0x70008210,0x70008310,0x70008410,0x70008510,0x70008610,0x70008710,0x70008810,0x70008910,0x70008A10,0x70008B10,0x70008C10,0x70008D10,0x70008E10,0x70008F10,0x70009010,0x70009110,0x70009210,0x70009310,0x70009410,0x70009510,0x70009610,0x70009710,0x70009810,0x70009910,0x70009A10,0x70009B10,0x70009C10,0x70009D10,0x70009E10,0x70009F10,0x7000A010,0x7000A110,0x7000A210,0x7000A310,0x7000A410,0x7000A510,0x7000A610,0x7000A710,0x7000A810,0x7000A910,0x7000AA10,0x7000AB10,0x7000AC10,0x7000AD10,0x7000AE10,0x7000AF10,0x7000B010,0x7000B110,0x7000B210,0x7000B310,0x7000B410,0x7000B510,0x7000B610,0x7000B710,0x7000B810,0x7000B910,0x7000BA10,0x7000BB10,0x7000BC10,0x7000BD10,0x7000BE10,0x7000BF10,0x7000C010,0x7000C110,0x7000C210,0x7000C310,0x7000C410,0x7000C510,0x7000C610,0x7000C710,0x7000C810,0x7000C910,0x7000CA10,0x7000CB10,0x7000CC10,0x7000CD10,0x7000CE10,0x7000CF10,0x7000D010,0x7000D110,0x7000D210,0x7000D310,0x7000D410,0x7000D510,0x7000D610,0x7000D710,0x7000D810,0x7000D910,0x7000DA10,0x7000DB10,0x7000DC10,0x7000DD10,0x7000DE10,0x7000DF10,0x7000E010,0x7000E110,0x7000E210,0x7000E310,0x7000E410,0x7000E510,0x7000E610,0x7000E710,0x7000E810,0x7000E910,0x7000EA10,0x7000EB10,0x7000EC10,0x7000ED10,0x7000EE10,0x7000EF10,0x7000F010,0x7000F110,0x7000F210,0x7000F310,0x7000F410,0x7000F510,0x7000F610,0x7000F710,0x7000F810,0x7000F910,0x7000FA10,0x7000FB10,0x7000FC10,0x7000FD10,0x7000FE10,0x7000FF10}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1601 : DO Unit4
******************************************************************************/
/**
* \addtogroup 0x1601 0x1601 | DO Unit4
* @{
* \brief Object 0x1601 (DO Unit4) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
* SubIndex 65<br>
* SubIndex 66<br>
* SubIndex 67<br>
* SubIndex 68<br>
* SubIndex 69<br>
* SubIndex 70<br>
* SubIndex 71<br>
* SubIndex 72<br>
* SubIndex 73<br>
* SubIndex 74<br>
* SubIndex 75<br>
* SubIndex 76<br>
* SubIndex 77<br>
* SubIndex 78<br>
* SubIndex 79<br>
* SubIndex 80<br>
* SubIndex 81<br>
* SubIndex 82<br>
* SubIndex 83<br>
* SubIndex 84<br>
* SubIndex 85<br>
* SubIndex 86<br>
* SubIndex 87<br>
* SubIndex 88<br>
* SubIndex 89<br>
* SubIndex 90<br>
* SubIndex 91<br>
* SubIndex 92<br>
* SubIndex 93<br>
* SubIndex 94<br>
* SubIndex 95<br>
* SubIndex 96<br>
* SubIndex 97<br>
* SubIndex 98<br>
* SubIndex 99<br>
* SubIndex 100<br>
* SubIndex 101<br>
* SubIndex 102<br>
* SubIndex 103<br>
* SubIndex 104<br>
* SubIndex 105<br>
* SubIndex 106<br>
* SubIndex 107<br>
* SubIndex 108<br>
* SubIndex 109<br>
* SubIndex 110<br>
* SubIndex 111<br>
* SubIndex 112<br>
* SubIndex 113<br>
* SubIndex 114<br>
* SubIndex 115<br>
* SubIndex 116<br>
* SubIndex 117<br>
* SubIndex 118<br>
* SubIndex 119<br>
* SubIndex 120<br>
* SubIndex 121<br>
* SubIndex 122<br>
* SubIndex 123<br>
* SubIndex 124<br>
* SubIndex 125<br>
* SubIndex 126<br>
* SubIndex 127<br>
* SubIndex 128<br>
* SubIndex 129<br>
* SubIndex 130<br>
* SubIndex 131<br>
* SubIndex 132<br>
* SubIndex 133<br>
* SubIndex 134<br>
* SubIndex 135<br>
* SubIndex 136<br>
* SubIndex 137<br>
* SubIndex 138<br>
* SubIndex 139<br>
* SubIndex 140<br>
* SubIndex 141<br>
* SubIndex 142<br>
* SubIndex 143<br>
* SubIndex 144<br>
* SubIndex 145<br>
* SubIndex 146<br>
* SubIndex 147<br>
* SubIndex 148<br>
* SubIndex 149<br>
* SubIndex 150<br>
* SubIndex 151<br>
* SubIndex 152<br>
* SubIndex 153<br>
* SubIndex 154<br>
* SubIndex 155<br>
* SubIndex 156<br>
* SubIndex 157<br>
* SubIndex 158<br>
* SubIndex 159<br>
* SubIndex 160<br>
* SubIndex 161<br>
* SubIndex 162<br>
* SubIndex 163<br>
* SubIndex 164<br>
* SubIndex 165<br>
* SubIndex 166<br>
* SubIndex 167<br>
* SubIndex 168<br>
* SubIndex 169<br>
* SubIndex 170<br>
* SubIndex 171<br>
* SubIndex 172<br>
* SubIndex 173<br>
* SubIndex 174<br>
* SubIndex 175<br>
* SubIndex 176<br>
* SubIndex 177<br>
* SubIndex 178<br>
* SubIndex 179<br>
* SubIndex 180<br>
* SubIndex 181<br>
* SubIndex 182<br>
* SubIndex 183<br>
* SubIndex 184<br>
* SubIndex 185<br>
* SubIndex 186<br>
* SubIndex 187<br>
* SubIndex 188<br>
* SubIndex 189<br>
* SubIndex 190<br>
* SubIndex 191<br>
* SubIndex 192<br>
* SubIndex 193<br>
* SubIndex 194<br>
* SubIndex 195<br>
* SubIndex 196<br>
* SubIndex 197<br>
* SubIndex 198<br>
* SubIndex 199<br>
* SubIndex 200<br>
* SubIndex 201<br>
* SubIndex 202<br>
* SubIndex 203<br>
* SubIndex 204<br>
* SubIndex 205<br>
* SubIndex 206<br>
* SubIndex 207<br>
* SubIndex 208<br>
* SubIndex 209<br>
* SubIndex 210<br>
* SubIndex 211<br>
* SubIndex 212<br>
* SubIndex 213<br>
* SubIndex 214<br>
* SubIndex 215<br>
* SubIndex 216<br>
* SubIndex 217<br>
* SubIndex 218<br>
* SubIndex 219<br>
* SubIndex 220<br>
* SubIndex 221<br>
* SubIndex 222<br>
* SubIndex 223<br>
* SubIndex 224<br>
* SubIndex 225<br>
* SubIndex 226<br>
* SubIndex 227<br>
* SubIndex 228<br>
* SubIndex 229<br>
* SubIndex 230<br>
* SubIndex 231<br>
* SubIndex 232<br>
* SubIndex 233<br>
* SubIndex 234<br>
* SubIndex 235<br>
* SubIndex 236<br>
* SubIndex 237<br>
* SubIndex 238<br>
* SubIndex 239<br>
* SubIndex 240<br>
* SubIndex 241<br>
* SubIndex 242<br>
* SubIndex 243<br>
* SubIndex 244<br>
* SubIndex 245<br>
* SubIndex 246<br>
* SubIndex 247<br>
* SubIndex 248<br>
* SubIndex 249<br>
* SubIndex 250<br>
* SubIndex 251<br>
* SubIndex 252<br>
* SubIndex 253<br>
* SubIndex 254<br>
* SubIndex 255<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1601[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex64 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex65 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex66 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex67 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex68 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex69 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex70 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex71 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex72 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex73 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex74 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex75 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex76 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex77 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex78 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex79 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex80 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex81 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex82 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex83 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex84 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex85 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex86 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex87 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex88 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex89 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex90 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex91 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex92 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex93 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex94 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex95 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex96 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex97 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex98 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex99 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex100 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex101 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex102 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex103 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex104 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex105 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex106 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex107 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex108 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex109 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex110 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex111 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex112 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex113 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex114 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex115 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex116 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex117 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex118 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex119 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex120 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex121 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex122 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex123 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex124 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex125 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex126 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex127 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex128 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex129 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex130 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex131 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex132 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex133 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex134 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex135 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex136 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex137 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex138 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex139 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex140 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex141 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex142 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex143 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex144 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex145 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex146 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex147 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex148 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex149 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex150 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex151 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex152 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex153 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex154 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex155 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex156 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex157 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex158 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex159 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex160 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex161 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex162 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex163 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex164 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex165 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex166 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex167 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex168 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex169 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex170 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex171 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex172 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex173 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex174 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex175 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex176 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex177 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex178 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex179 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex180 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex181 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex182 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex183 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex184 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex185 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex186 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex187 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex188 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex189 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex190 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex191 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex192 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex193 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex194 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex195 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex196 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex197 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex198 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex199 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex200 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex201 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex202 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex203 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex204 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex205 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex206 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex207 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex208 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex209 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex210 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex211 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex212 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex213 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex214 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex215 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex216 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex217 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex218 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex219 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex220 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex221 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex222 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex223 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex224 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex225 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex226 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex227 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex228 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex229 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex230 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex231 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex232 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex233 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex234 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex235 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex236 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex237 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex238 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex239 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex240 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex241 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex242 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex243 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex244 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex245 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex246 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex247 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex248 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex249 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex250 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex251 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex252 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex253 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex254 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex255 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1601[] = "DO Unit4\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000"
"SubIndex 014\000"
"SubIndex 015\000"
"SubIndex 016\000"
"SubIndex 017\000"
"SubIndex 018\000"
"SubIndex 019\000"
"SubIndex 020\000"
"SubIndex 021\000"
"SubIndex 022\000"
"SubIndex 023\000"
"SubIndex 024\000"
"SubIndex 025\000"
"SubIndex 026\000"
"SubIndex 027\000"
"SubIndex 028\000"
"SubIndex 029\000"
"SubIndex 030\000"
"SubIndex 031\000"
"SubIndex 032\000"
"SubIndex 033\000"
"SubIndex 034\000"
"SubIndex 035\000"
"SubIndex 036\000"
"SubIndex 037\000"
"SubIndex 038\000"
"SubIndex 039\000"
"SubIndex 040\000"
"SubIndex 041\000"
"SubIndex 042\000"
"SubIndex 043\000"
"SubIndex 044\000"
"SubIndex 045\000"
"SubIndex 046\000"
"SubIndex 047\000"
"SubIndex 048\000"
"SubIndex 049\000"
"SubIndex 050\000"
"SubIndex 051\000"
"SubIndex 052\000"
"SubIndex 053\000"
"SubIndex 054\000"
"SubIndex 055\000"
"SubIndex 056\000"
"SubIndex 057\000"
"SubIndex 058\000"
"SubIndex 059\000"
"SubIndex 060\000"
"SubIndex 061\000"
"SubIndex 062\000"
"SubIndex 063\000"
"SubIndex 064\000"
"SubIndex 065\000"
"SubIndex 066\000"
"SubIndex 067\000"
"SubIndex 068\000"
"SubIndex 069\000"
"SubIndex 070\000"
"SubIndex 071\000"
"SubIndex 072\000"
"SubIndex 073\000"
"SubIndex 074\000"
"SubIndex 075\000"
"SubIndex 076\000"
"SubIndex 077\000"
"SubIndex 078\000"
"SubIndex 079\000"
"SubIndex 080\000"
"SubIndex 081\000"
"SubIndex 082\000"
"SubIndex 083\000"
"SubIndex 084\000"
"SubIndex 085\000"
"SubIndex 086\000"
"SubIndex 087\000"
"SubIndex 088\000"
"SubIndex 089\000"
"SubIndex 090\000"
"SubIndex 091\000"
"SubIndex 092\000"
"SubIndex 093\000"
"SubIndex 094\000"
"SubIndex 095\000"
"SubIndex 096\000"
"SubIndex 097\000"
"SubIndex 098\000"
"SubIndex 099\000"
"SubIndex 100\000"
"SubIndex 101\000"
"SubIndex 102\000"
"SubIndex 103\000"
"SubIndex 104\000"
"SubIndex 105\000"
"SubIndex 106\000"
"SubIndex 107\000"
"SubIndex 108\000"
"SubIndex 109\000"
"SubIndex 110\000"
"SubIndex 111\000"
"SubIndex 112\000"
"SubIndex 113\000"
"SubIndex 114\000"
"SubIndex 115\000"
"SubIndex 116\000"
"SubIndex 117\000"
"SubIndex 118\000"
"SubIndex 119\000"
"SubIndex 120\000"
"SubIndex 121\000"
"SubIndex 122\000"
"SubIndex 123\000"
"SubIndex 124\000"
"SubIndex 125\000"
"SubIndex 126\000"
"SubIndex 127\000"
"SubIndex 128\000"
"SubIndex 129\000"
"SubIndex 130\000"
"SubIndex 131\000"
"SubIndex 132\000"
"SubIndex 133\000"
"SubIndex 134\000"
"SubIndex 135\000"
"SubIndex 136\000"
"SubIndex 137\000"
"SubIndex 138\000"
"SubIndex 139\000"
"SubIndex 140\000"
"SubIndex 141\000"
"SubIndex 142\000"
"SubIndex 143\000"
"SubIndex 144\000"
"SubIndex 145\000"
"SubIndex 146\000"
"SubIndex 147\000"
"SubIndex 148\000"
"SubIndex 149\000"
"SubIndex 150\000"
"SubIndex 151\000"
"SubIndex 152\000"
"SubIndex 153\000"
"SubIndex 154\000"
"SubIndex 155\000"
"SubIndex 156\000"
"SubIndex 157\000"
"SubIndex 158\000"
"SubIndex 159\000"
"SubIndex 160\000"
"SubIndex 161\000"
"SubIndex 162\000"
"SubIndex 163\000"
"SubIndex 164\000"
"SubIndex 165\000"
"SubIndex 166\000"
"SubIndex 167\000"
"SubIndex 168\000"
"SubIndex 169\000"
"SubIndex 170\000"
"SubIndex 171\000"
"SubIndex 172\000"
"SubIndex 173\000"
"SubIndex 174\000"
"SubIndex 175\000"
"SubIndex 176\000"
"SubIndex 177\000"
"SubIndex 178\000"
"SubIndex 179\000"
"SubIndex 180\000"
"SubIndex 181\000"
"SubIndex 182\000"
"SubIndex 183\000"
"SubIndex 184\000"
"SubIndex 185\000"
"SubIndex 186\000"
"SubIndex 187\000"
"SubIndex 188\000"
"SubIndex 189\000"
"SubIndex 190\000"
"SubIndex 191\000"
"SubIndex 192\000"
"SubIndex 193\000"
"SubIndex 194\000"
"SubIndex 195\000"
"SubIndex 196\000"
"SubIndex 197\000"
"SubIndex 198\000"
"SubIndex 199\000"
"SubIndex 200\000"
"SubIndex 201\000"
"SubIndex 202\000"
"SubIndex 203\000"
"SubIndex 204\000"
"SubIndex 205\000"
"SubIndex 206\000"
"SubIndex 207\000"
"SubIndex 208\000"
"SubIndex 209\000"
"SubIndex 210\000"
"SubIndex 211\000"
"SubIndex 212\000"
"SubIndex 213\000"
"SubIndex 214\000"
"SubIndex 215\000"
"SubIndex 216\000"
"SubIndex 217\000"
"SubIndex 218\000"
"SubIndex 219\000"
"SubIndex 220\000"
"SubIndex 221\000"
"SubIndex 222\000"
"SubIndex 223\000"
"SubIndex 224\000"
"SubIndex 225\000"
"SubIndex 226\000"
"SubIndex 227\000"
"SubIndex 228\000"
"SubIndex 229\000"
"SubIndex 230\000"
"SubIndex 231\000"
"SubIndex 232\000"
"SubIndex 233\000"
"SubIndex 234\000"
"SubIndex 235\000"
"SubIndex 236\000"
"SubIndex 237\000"
"SubIndex 238\000"
"SubIndex 239\000"
"SubIndex 240\000"
"SubIndex 241\000"
"SubIndex 242\000"
"SubIndex 243\000"
"SubIndex 244\000"
"SubIndex 245\000"
"SubIndex 246\000"
"SubIndex 247\000"
"SubIndex 248\000"
"SubIndex 249\000"
"SubIndex 250\000"
"SubIndex 251\000"
"SubIndex 252\000"
"SubIndex 253\000"
"SubIndex 254\000"
"SubIndex 255\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
UINT32 SI65; /* Subindex65 -  */
UINT32 SI66; /* Subindex66 -  */
UINT32 SI67; /* Subindex67 -  */
UINT32 SI68; /* Subindex68 -  */
UINT32 SI69; /* Subindex69 -  */
UINT32 SI70; /* Subindex70 -  */
UINT32 SI71; /* Subindex71 -  */
UINT32 SI72; /* Subindex72 -  */
UINT32 SI73; /* Subindex73 -  */
UINT32 SI74; /* Subindex74 -  */
UINT32 SI75; /* Subindex75 -  */
UINT32 SI76; /* Subindex76 -  */
UINT32 SI77; /* Subindex77 -  */
UINT32 SI78; /* Subindex78 -  */
UINT32 SI79; /* Subindex79 -  */
UINT32 SI80; /* Subindex80 -  */
UINT32 SI81; /* Subindex81 -  */
UINT32 SI82; /* Subindex82 -  */
UINT32 SI83; /* Subindex83 -  */
UINT32 SI84; /* Subindex84 -  */
UINT32 SI85; /* Subindex85 -  */
UINT32 SI86; /* Subindex86 -  */
UINT32 SI87; /* Subindex87 -  */
UINT32 SI88; /* Subindex88 -  */
UINT32 SI89; /* Subindex89 -  */
UINT32 SI90; /* Subindex90 -  */
UINT32 SI91; /* Subindex91 -  */
UINT32 SI92; /* Subindex92 -  */
UINT32 SI93; /* Subindex93 -  */
UINT32 SI94; /* Subindex94 -  */
UINT32 SI95; /* Subindex95 -  */
UINT32 SI96; /* Subindex96 -  */
UINT32 SI97; /* Subindex97 -  */
UINT32 SI98; /* Subindex98 -  */
UINT32 SI99; /* Subindex99 -  */
UINT32 SI100; /* Subindex100 -  */
UINT32 SI101; /* Subindex101 -  */
UINT32 SI102; /* Subindex102 -  */
UINT32 SI103; /* Subindex103 -  */
UINT32 SI104; /* Subindex104 -  */
UINT32 SI105; /* Subindex105 -  */
UINT32 SI106; /* Subindex106 -  */
UINT32 SI107; /* Subindex107 -  */
UINT32 SI108; /* Subindex108 -  */
UINT32 SI109; /* Subindex109 -  */
UINT32 SI110; /* Subindex110 -  */
UINT32 SI111; /* Subindex111 -  */
UINT32 SI112; /* Subindex112 -  */
UINT32 SI113; /* Subindex113 -  */
UINT32 SI114; /* Subindex114 -  */
UINT32 SI115; /* Subindex115 -  */
UINT32 SI116; /* Subindex116 -  */
UINT32 SI117; /* Subindex117 -  */
UINT32 SI118; /* Subindex118 -  */
UINT32 SI119; /* Subindex119 -  */
UINT32 SI120; /* Subindex120 -  */
UINT32 SI121; /* Subindex121 -  */
UINT32 SI122; /* Subindex122 -  */
UINT32 SI123; /* Subindex123 -  */
UINT32 SI124; /* Subindex124 -  */
UINT32 SI125; /* Subindex125 -  */
UINT32 SI126; /* Subindex126 -  */
UINT32 SI127; /* Subindex127 -  */
UINT32 SI128; /* Subindex128 -  */
UINT32 SI129; /* Subindex129 -  */
UINT32 SI130; /* Subindex130 -  */
UINT32 SI131; /* Subindex131 -  */
UINT32 SI132; /* Subindex132 -  */
UINT32 SI133; /* Subindex133 -  */
UINT32 SI134; /* Subindex134 -  */
UINT32 SI135; /* Subindex135 -  */
UINT32 SI136; /* Subindex136 -  */
UINT32 SI137; /* Subindex137 -  */
UINT32 SI138; /* Subindex138 -  */
UINT32 SI139; /* Subindex139 -  */
UINT32 SI140; /* Subindex140 -  */
UINT32 SI141; /* Subindex141 -  */
UINT32 SI142; /* Subindex142 -  */
UINT32 SI143; /* Subindex143 -  */
UINT32 SI144; /* Subindex144 -  */
UINT32 SI145; /* Subindex145 -  */
UINT32 SI146; /* Subindex146 -  */
UINT32 SI147; /* Subindex147 -  */
UINT32 SI148; /* Subindex148 -  */
UINT32 SI149; /* Subindex149 -  */
UINT32 SI150; /* Subindex150 -  */
UINT32 SI151; /* Subindex151 -  */
UINT32 SI152; /* Subindex152 -  */
UINT32 SI153; /* Subindex153 -  */
UINT32 SI154; /* Subindex154 -  */
UINT32 SI155; /* Subindex155 -  */
UINT32 SI156; /* Subindex156 -  */
UINT32 SI157; /* Subindex157 -  */
UINT32 SI158; /* Subindex158 -  */
UINT32 SI159; /* Subindex159 -  */
UINT32 SI160; /* Subindex160 -  */
UINT32 SI161; /* Subindex161 -  */
UINT32 SI162; /* Subindex162 -  */
UINT32 SI163; /* Subindex163 -  */
UINT32 SI164; /* Subindex164 -  */
UINT32 SI165; /* Subindex165 -  */
UINT32 SI166; /* Subindex166 -  */
UINT32 SI167; /* Subindex167 -  */
UINT32 SI168; /* Subindex168 -  */
UINT32 SI169; /* Subindex169 -  */
UINT32 SI170; /* Subindex170 -  */
UINT32 SI171; /* Subindex171 -  */
UINT32 SI172; /* Subindex172 -  */
UINT32 SI173; /* Subindex173 -  */
UINT32 SI174; /* Subindex174 -  */
UINT32 SI175; /* Subindex175 -  */
UINT32 SI176; /* Subindex176 -  */
UINT32 SI177; /* Subindex177 -  */
UINT32 SI178; /* Subindex178 -  */
UINT32 SI179; /* Subindex179 -  */
UINT32 SI180; /* Subindex180 -  */
UINT32 SI181; /* Subindex181 -  */
UINT32 SI182; /* Subindex182 -  */
UINT32 SI183; /* Subindex183 -  */
UINT32 SI184; /* Subindex184 -  */
UINT32 SI185; /* Subindex185 -  */
UINT32 SI186; /* Subindex186 -  */
UINT32 SI187; /* Subindex187 -  */
UINT32 SI188; /* Subindex188 -  */
UINT32 SI189; /* Subindex189 -  */
UINT32 SI190; /* Subindex190 -  */
UINT32 SI191; /* Subindex191 -  */
UINT32 SI192; /* Subindex192 -  */
UINT32 SI193; /* Subindex193 -  */
UINT32 SI194; /* Subindex194 -  */
UINT32 SI195; /* Subindex195 -  */
UINT32 SI196; /* Subindex196 -  */
UINT32 SI197; /* Subindex197 -  */
UINT32 SI198; /* Subindex198 -  */
UINT32 SI199; /* Subindex199 -  */
UINT32 SI200; /* Subindex200 -  */
UINT32 SI201; /* Subindex201 -  */
UINT32 SI202; /* Subindex202 -  */
UINT32 SI203; /* Subindex203 -  */
UINT32 SI204; /* Subindex204 -  */
UINT32 SI205; /* Subindex205 -  */
UINT32 SI206; /* Subindex206 -  */
UINT32 SI207; /* Subindex207 -  */
UINT32 SI208; /* Subindex208 -  */
UINT32 SI209; /* Subindex209 -  */
UINT32 SI210; /* Subindex210 -  */
UINT32 SI211; /* Subindex211 -  */
UINT32 SI212; /* Subindex212 -  */
UINT32 SI213; /* Subindex213 -  */
UINT32 SI214; /* Subindex214 -  */
UINT32 SI215; /* Subindex215 -  */
UINT32 SI216; /* Subindex216 -  */
UINT32 SI217; /* Subindex217 -  */
UINT32 SI218; /* Subindex218 -  */
UINT32 SI219; /* Subindex219 -  */
UINT32 SI220; /* Subindex220 -  */
UINT32 SI221; /* Subindex221 -  */
UINT32 SI222; /* Subindex222 -  */
UINT32 SI223; /* Subindex223 -  */
UINT32 SI224; /* Subindex224 -  */
UINT32 SI225; /* Subindex225 -  */
UINT32 SI226; /* Subindex226 -  */
UINT32 SI227; /* Subindex227 -  */
UINT32 SI228; /* Subindex228 -  */
UINT32 SI229; /* Subindex229 -  */
UINT32 SI230; /* Subindex230 -  */
UINT32 SI231; /* Subindex231 -  */
UINT32 SI232; /* Subindex232 -  */
UINT32 SI233; /* Subindex233 -  */
UINT32 SI234; /* Subindex234 -  */
UINT32 SI235; /* Subindex235 -  */
UINT32 SI236; /* Subindex236 -  */
UINT32 SI237; /* Subindex237 -  */
UINT32 SI238; /* Subindex238 -  */
UINT32 SI239; /* Subindex239 -  */
UINT32 SI240; /* Subindex240 -  */
UINT32 SI241; /* Subindex241 -  */
UINT32 SI242; /* Subindex242 -  */
UINT32 SI243; /* Subindex243 -  */
UINT32 SI244; /* Subindex244 -  */
UINT32 SI245; /* Subindex245 -  */
UINT32 SI246; /* Subindex246 -  */
UINT32 SI247; /* Subindex247 -  */
UINT32 SI248; /* Subindex248 -  */
UINT32 SI249; /* Subindex249 -  */
UINT32 SI250; /* Subindex250 -  */
UINT32 SI251; /* Subindex251 -  */
UINT32 SI252; /* Subindex252 -  */
UINT32 SI253; /* Subindex253 -  */
UINT32 SI254; /* Subindex254 -  */
UINT32 SI255; /* Subindex255 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1601;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1601 DOUnit40x1601
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={255,0x70010110,0x70010210,0x70010310,0x70010410,0x70010510,0x70010610,0x70010710,0x70010810,0x70010910,0x70010A10,0x70010B10,0x70010C10,0x70010D10,0x70010E10,0x70010F10,0x70011010,0x70011110,0x70011210,0x70011310,0x70011410,0x70011510,0x70011610,0x70011710,0x70011810,0x70011910,0x70011A10,0x70011B10,0x70011C10,0x70011D10,0x70011E10,0x70011F10,0x70012010,0x70012110,0x70012210,0x70012310,0x70012410,0x70012510,0x70012610,0x70012710,0x70012810,0x70012910,0x70012A10,0x70012B10,0x70012C10,0x70012D10,0x70012E10,0x70012F10,0x70013010,0x70013110,0x70013210,0x70013310,0x70013410,0x70013510,0x70013610,0x70013710,0x70013810,0x70013910,0x70013A10,0x70013B10,0x70013C10,0x70013D10,0x70013E10,0x70013F10,0x70014010,0x70014110,0x70014210,0x70014310,0x70014410,0x70014510,0x70014610,0x70014710,0x70014810,0x70014910,0x70014A10,0x70014B10,0x70014C10,0x70014D10,0x70014E10,0x70014F10,0x70015010,0x70015110,0x70015210,0x70015310,0x70015410,0x70015510,0x70015610,0x70015710,0x70015810,0x70015910,0x70015A10,0x70015B10,0x70015C10,0x70015D10,0x70015E10,0x70015F10,0x70016010,0x70016110,0x70016210,0x70016310,0x70016410,0x70016510,0x70016610,0x70016710,0x70016810,0x70016910,0x70016A10,0x70016B10,0x70016C10,0x70016D10,0x70016E10,0x70016F10,0x70017010,0x70017110,0x70017210,0x70017310,0x70017410,0x70017510,0x70017610,0x70017710,0x70017810,0x70017910,0x70017A10,0x70017B10,0x70017C10,0x70017D10,0x70017E10,0x70017F10,0x70018010,0x70018110,0x70018210,0x70018310,0x70018410,0x70018510,0x70018610,0x70018710,0x70018810,0x70018910,0x70018A10,0x70018B10,0x70018C10,0x70018D10,0x70018E10,0x70018F10,0x70019010,0x70019110,0x70019210,0x70019310,0x70019410,0x70019510,0x70019610,0x70019710,0x70019810,0x70019910,0x70019A10,0x70019B10,0x70019C10,0x70019D10,0x70019E10,0x70019F10,0x7001A010,0x7001A110,0x7001A210,0x7001A310,0x7001A410,0x7001A510,0x7001A610,0x7001A710,0x7001A810,0x7001A910,0x7001AA10,0x7001AB10,0x7001AC10,0x7001AD10,0x7001AE10,0x7001AF10,0x7001B010,0x7001B110,0x7001B210,0x7001B310,0x7001B410,0x7001B510,0x7001B610,0x7001B710,0x7001B810,0x7001B910,0x7001BA10,0x7001BB10,0x7001BC10,0x7001BD10,0x7001BE10,0x7001BF10,0x7001C010,0x7001C110,0x7001C210,0x7001C310,0x7001C410,0x7001C510,0x7001C610,0x7001C710,0x7001C810,0x7001C910,0x7001CA10,0x7001CB10,0x7001CC10,0x7001CD10,0x7001CE10,0x7001CF10,0x7001D010,0x7001D110,0x7001D210,0x7001D310,0x7001D410,0x7001D510,0x7001D610,0x7001D710,0x7001D810,0x7001D910,0x7001DA10,0x7001DB10,0x7001DC10,0x7001DD10,0x7001DE10,0x7001DF10,0x7001E010,0x7001E110,0x7001E210,0x7001E310,0x7001E410,0x7001E510,0x7001E610,0x7001E710,0x7001E810,0x7001E910,0x7001EA10,0x7001EB10,0x7001EC10,0x7001ED10,0x7001EE10,0x7001EF10,0x7001F010,0x7001F110,0x7001F210,0x7001F310,0x7001F410,0x7001F510,0x7001F610,0x7001F710,0x7001F810,0x7001F910,0x7001FA10,0x7001FB10,0x7001FC10,0x7001FD10,0x7001FE10,0x7001FF10}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A00 : DI Unit1
******************************************************************************/
/**
* \addtogroup 0x1A00 0x1A00 | DI Unit1
* @{
* \brief Object 0x1A00 (DI Unit1) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
* SubIndex 65<br>
* SubIndex 66<br>
* SubIndex 67<br>
* SubIndex 68<br>
* SubIndex 69<br>
* SubIndex 70<br>
* SubIndex 71<br>
* SubIndex 72<br>
* SubIndex 73<br>
* SubIndex 74<br>
* SubIndex 75<br>
* SubIndex 76<br>
* SubIndex 77<br>
* SubIndex 78<br>
* SubIndex 79<br>
* SubIndex 80<br>
* SubIndex 81<br>
* SubIndex 82<br>
* SubIndex 83<br>
* SubIndex 84<br>
* SubIndex 85<br>
* SubIndex 86<br>
* SubIndex 87<br>
* SubIndex 88<br>
* SubIndex 89<br>
* SubIndex 90<br>
* SubIndex 91<br>
* SubIndex 92<br>
* SubIndex 93<br>
* SubIndex 94<br>
* SubIndex 95<br>
* SubIndex 96<br>
* SubIndex 97<br>
* SubIndex 98<br>
* SubIndex 99<br>
* SubIndex 100<br>
* SubIndex 101<br>
* SubIndex 102<br>
* SubIndex 103<br>
* SubIndex 104<br>
* SubIndex 105<br>
* SubIndex 106<br>
* SubIndex 107<br>
* SubIndex 108<br>
* SubIndex 109<br>
* SubIndex 110<br>
* SubIndex 111<br>
* SubIndex 112<br>
* SubIndex 113<br>
* SubIndex 114<br>
* SubIndex 115<br>
* SubIndex 116<br>
* SubIndex 117<br>
* SubIndex 118<br>
* SubIndex 119<br>
* SubIndex 120<br>
* SubIndex 121<br>
* SubIndex 122<br>
* SubIndex 123<br>
* SubIndex 124<br>
* SubIndex 125<br>
* SubIndex 126<br>
* SubIndex 127<br>
* SubIndex 128<br>
* SubIndex 129<br>
* SubIndex 130<br>
* SubIndex 131<br>
* SubIndex 132<br>
* SubIndex 133<br>
* SubIndex 134<br>
* SubIndex 135<br>
* SubIndex 136<br>
* SubIndex 137<br>
* SubIndex 138<br>
* SubIndex 139<br>
* SubIndex 140<br>
* SubIndex 141<br>
* SubIndex 142<br>
* SubIndex 143<br>
* SubIndex 144<br>
* SubIndex 145<br>
* SubIndex 146<br>
* SubIndex 147<br>
* SubIndex 148<br>
* SubIndex 149<br>
* SubIndex 150<br>
* SubIndex 151<br>
* SubIndex 152<br>
* SubIndex 153<br>
* SubIndex 154<br>
* SubIndex 155<br>
* SubIndex 156<br>
* SubIndex 157<br>
* SubIndex 158<br>
* SubIndex 159<br>
* SubIndex 160<br>
* SubIndex 161<br>
* SubIndex 162<br>
* SubIndex 163<br>
* SubIndex 164<br>
* SubIndex 165<br>
* SubIndex 166<br>
* SubIndex 167<br>
* SubIndex 168<br>
* SubIndex 169<br>
* SubIndex 170<br>
* SubIndex 171<br>
* SubIndex 172<br>
* SubIndex 173<br>
* SubIndex 174<br>
* SubIndex 175<br>
* SubIndex 176<br>
* SubIndex 177<br>
* SubIndex 178<br>
* SubIndex 179<br>
* SubIndex 180<br>
* SubIndex 181<br>
* SubIndex 182<br>
* SubIndex 183<br>
* SubIndex 184<br>
* SubIndex 185<br>
* SubIndex 186<br>
* SubIndex 187<br>
* SubIndex 188<br>
* SubIndex 189<br>
* SubIndex 190<br>
* SubIndex 191<br>
* SubIndex 192<br>
* SubIndex 193<br>
* SubIndex 194<br>
* SubIndex 195<br>
* SubIndex 196<br>
* SubIndex 197<br>
* SubIndex 198<br>
* SubIndex 199<br>
* SubIndex 200<br>
* SubIndex 201<br>
* SubIndex 202<br>
* SubIndex 203<br>
* SubIndex 204<br>
* SubIndex 205<br>
* SubIndex 206<br>
* SubIndex 207<br>
* SubIndex 208<br>
* SubIndex 209<br>
* SubIndex 210<br>
* SubIndex 211<br>
* SubIndex 212<br>
* SubIndex 213<br>
* SubIndex 214<br>
* SubIndex 215<br>
* SubIndex 216<br>
* SubIndex 217<br>
* SubIndex 218<br>
* SubIndex 219<br>
* SubIndex 220<br>
* SubIndex 221<br>
* SubIndex 222<br>
* SubIndex 223<br>
* SubIndex 224<br>
* SubIndex 225<br>
* SubIndex 226<br>
* SubIndex 227<br>
* SubIndex 228<br>
* SubIndex 229<br>
* SubIndex 230<br>
* SubIndex 231<br>
* SubIndex 232<br>
* SubIndex 233<br>
* SubIndex 234<br>
* SubIndex 235<br>
* SubIndex 236<br>
* SubIndex 237<br>
* SubIndex 238<br>
* SubIndex 239<br>
* SubIndex 240<br>
* SubIndex 241<br>
* SubIndex 242<br>
* SubIndex 243<br>
* SubIndex 244<br>
* SubIndex 245<br>
* SubIndex 246<br>
* SubIndex 247<br>
* SubIndex 248<br>
* SubIndex 249<br>
* SubIndex 250<br>
* SubIndex 251<br>
* SubIndex 252<br>
* SubIndex 253<br>
* SubIndex 254<br>
* SubIndex 255<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A00[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex64 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex65 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex66 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex67 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex68 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex69 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex70 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex71 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex72 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex73 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex74 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex75 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex76 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex77 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex78 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex79 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex80 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex81 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex82 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex83 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex84 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex85 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex86 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex87 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex88 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex89 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex90 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex91 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex92 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex93 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex94 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex95 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex96 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex97 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex98 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex99 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex100 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex101 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex102 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex103 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex104 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex105 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex106 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex107 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex108 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex109 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex110 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex111 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex112 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex113 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex114 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex115 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex116 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex117 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex118 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex119 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex120 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex121 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex122 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex123 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex124 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex125 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex126 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex127 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex128 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex129 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex130 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex131 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex132 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex133 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex134 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex135 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex136 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex137 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex138 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex139 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex140 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex141 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex142 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex143 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex144 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex145 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex146 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex147 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex148 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex149 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex150 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex151 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex152 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex153 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex154 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex155 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex156 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex157 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex158 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex159 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex160 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex161 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex162 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex163 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex164 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex165 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex166 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex167 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex168 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex169 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex170 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex171 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex172 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex173 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex174 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex175 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex176 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex177 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex178 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex179 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex180 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex181 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex182 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex183 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex184 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex185 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex186 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex187 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex188 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex189 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex190 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex191 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex192 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex193 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex194 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex195 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex196 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex197 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex198 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex199 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex200 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex201 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex202 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex203 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex204 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex205 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex206 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex207 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex208 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex209 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex210 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex211 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex212 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex213 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex214 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex215 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex216 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex217 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex218 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex219 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex220 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex221 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex222 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex223 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex224 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex225 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex226 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex227 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex228 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex229 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex230 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex231 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex232 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex233 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex234 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex235 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex236 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex237 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex238 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex239 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex240 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex241 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex242 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex243 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex244 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex245 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex246 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex247 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex248 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex249 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex250 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex251 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex252 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex253 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex254 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex255 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A00[] = "DI Unit1\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000"
"SubIndex 014\000"
"SubIndex 015\000"
"SubIndex 016\000"
"SubIndex 017\000"
"SubIndex 018\000"
"SubIndex 019\000"
"SubIndex 020\000"
"SubIndex 021\000"
"SubIndex 022\000"
"SubIndex 023\000"
"SubIndex 024\000"
"SubIndex 025\000"
"SubIndex 026\000"
"SubIndex 027\000"
"SubIndex 028\000"
"SubIndex 029\000"
"SubIndex 030\000"
"SubIndex 031\000"
"SubIndex 032\000"
"SubIndex 033\000"
"SubIndex 034\000"
"SubIndex 035\000"
"SubIndex 036\000"
"SubIndex 037\000"
"SubIndex 038\000"
"SubIndex 039\000"
"SubIndex 040\000"
"SubIndex 041\000"
"SubIndex 042\000"
"SubIndex 043\000"
"SubIndex 044\000"
"SubIndex 045\000"
"SubIndex 046\000"
"SubIndex 047\000"
"SubIndex 048\000"
"SubIndex 049\000"
"SubIndex 050\000"
"SubIndex 051\000"
"SubIndex 052\000"
"SubIndex 053\000"
"SubIndex 054\000"
"SubIndex 055\000"
"SubIndex 056\000"
"SubIndex 057\000"
"SubIndex 058\000"
"SubIndex 059\000"
"SubIndex 060\000"
"SubIndex 061\000"
"SubIndex 062\000"
"SubIndex 063\000"
"SubIndex 064\000"
"SubIndex 065\000"
"SubIndex 066\000"
"SubIndex 067\000"
"SubIndex 068\000"
"SubIndex 069\000"
"SubIndex 070\000"
"SubIndex 071\000"
"SubIndex 072\000"
"SubIndex 073\000"
"SubIndex 074\000"
"SubIndex 075\000"
"SubIndex 076\000"
"SubIndex 077\000"
"SubIndex 078\000"
"SubIndex 079\000"
"SubIndex 080\000"
"SubIndex 081\000"
"SubIndex 082\000"
"SubIndex 083\000"
"SubIndex 084\000"
"SubIndex 085\000"
"SubIndex 086\000"
"SubIndex 087\000"
"SubIndex 088\000"
"SubIndex 089\000"
"SubIndex 090\000"
"SubIndex 091\000"
"SubIndex 092\000"
"SubIndex 093\000"
"SubIndex 094\000"
"SubIndex 095\000"
"SubIndex 096\000"
"SubIndex 097\000"
"SubIndex 098\000"
"SubIndex 099\000"
"SubIndex 100\000"
"SubIndex 101\000"
"SubIndex 102\000"
"SubIndex 103\000"
"SubIndex 104\000"
"SubIndex 105\000"
"SubIndex 106\000"
"SubIndex 107\000"
"SubIndex 108\000"
"SubIndex 109\000"
"SubIndex 110\000"
"SubIndex 111\000"
"SubIndex 112\000"
"SubIndex 113\000"
"SubIndex 114\000"
"SubIndex 115\000"
"SubIndex 116\000"
"SubIndex 117\000"
"SubIndex 118\000"
"SubIndex 119\000"
"SubIndex 120\000"
"SubIndex 121\000"
"SubIndex 122\000"
"SubIndex 123\000"
"SubIndex 124\000"
"SubIndex 125\000"
"SubIndex 126\000"
"SubIndex 127\000"
"SubIndex 128\000"
"SubIndex 129\000"
"SubIndex 130\000"
"SubIndex 131\000"
"SubIndex 132\000"
"SubIndex 133\000"
"SubIndex 134\000"
"SubIndex 135\000"
"SubIndex 136\000"
"SubIndex 137\000"
"SubIndex 138\000"
"SubIndex 139\000"
"SubIndex 140\000"
"SubIndex 141\000"
"SubIndex 142\000"
"SubIndex 143\000"
"SubIndex 144\000"
"SubIndex 145\000"
"SubIndex 146\000"
"SubIndex 147\000"
"SubIndex 148\000"
"SubIndex 149\000"
"SubIndex 150\000"
"SubIndex 151\000"
"SubIndex 152\000"
"SubIndex 153\000"
"SubIndex 154\000"
"SubIndex 155\000"
"SubIndex 156\000"
"SubIndex 157\000"
"SubIndex 158\000"
"SubIndex 159\000"
"SubIndex 160\000"
"SubIndex 161\000"
"SubIndex 162\000"
"SubIndex 163\000"
"SubIndex 164\000"
"SubIndex 165\000"
"SubIndex 166\000"
"SubIndex 167\000"
"SubIndex 168\000"
"SubIndex 169\000"
"SubIndex 170\000"
"SubIndex 171\000"
"SubIndex 172\000"
"SubIndex 173\000"
"SubIndex 174\000"
"SubIndex 175\000"
"SubIndex 176\000"
"SubIndex 177\000"
"SubIndex 178\000"
"SubIndex 179\000"
"SubIndex 180\000"
"SubIndex 181\000"
"SubIndex 182\000"
"SubIndex 183\000"
"SubIndex 184\000"
"SubIndex 185\000"
"SubIndex 186\000"
"SubIndex 187\000"
"SubIndex 188\000"
"SubIndex 189\000"
"SubIndex 190\000"
"SubIndex 191\000"
"SubIndex 192\000"
"SubIndex 193\000"
"SubIndex 194\000"
"SubIndex 195\000"
"SubIndex 196\000"
"SubIndex 197\000"
"SubIndex 198\000"
"SubIndex 199\000"
"SubIndex 200\000"
"SubIndex 201\000"
"SubIndex 202\000"
"SubIndex 203\000"
"SubIndex 204\000"
"SubIndex 205\000"
"SubIndex 206\000"
"SubIndex 207\000"
"SubIndex 208\000"
"SubIndex 209\000"
"SubIndex 210\000"
"SubIndex 211\000"
"SubIndex 212\000"
"SubIndex 213\000"
"SubIndex 214\000"
"SubIndex 215\000"
"SubIndex 216\000"
"SubIndex 217\000"
"SubIndex 218\000"
"SubIndex 219\000"
"SubIndex 220\000"
"SubIndex 221\000"
"SubIndex 222\000"
"SubIndex 223\000"
"SubIndex 224\000"
"SubIndex 225\000"
"SubIndex 226\000"
"SubIndex 227\000"
"SubIndex 228\000"
"SubIndex 229\000"
"SubIndex 230\000"
"SubIndex 231\000"
"SubIndex 232\000"
"SubIndex 233\000"
"SubIndex 234\000"
"SubIndex 235\000"
"SubIndex 236\000"
"SubIndex 237\000"
"SubIndex 238\000"
"SubIndex 239\000"
"SubIndex 240\000"
"SubIndex 241\000"
"SubIndex 242\000"
"SubIndex 243\000"
"SubIndex 244\000"
"SubIndex 245\000"
"SubIndex 246\000"
"SubIndex 247\000"
"SubIndex 248\000"
"SubIndex 249\000"
"SubIndex 250\000"
"SubIndex 251\000"
"SubIndex 252\000"
"SubIndex 253\000"
"SubIndex 254\000"
"SubIndex 255\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
UINT32 SI65; /* Subindex65 -  */
UINT32 SI66; /* Subindex66 -  */
UINT32 SI67; /* Subindex67 -  */
UINT32 SI68; /* Subindex68 -  */
UINT32 SI69; /* Subindex69 -  */
UINT32 SI70; /* Subindex70 -  */
UINT32 SI71; /* Subindex71 -  */
UINT32 SI72; /* Subindex72 -  */
UINT32 SI73; /* Subindex73 -  */
UINT32 SI74; /* Subindex74 -  */
UINT32 SI75; /* Subindex75 -  */
UINT32 SI76; /* Subindex76 -  */
UINT32 SI77; /* Subindex77 -  */
UINT32 SI78; /* Subindex78 -  */
UINT32 SI79; /* Subindex79 -  */
UINT32 SI80; /* Subindex80 -  */
UINT32 SI81; /* Subindex81 -  */
UINT32 SI82; /* Subindex82 -  */
UINT32 SI83; /* Subindex83 -  */
UINT32 SI84; /* Subindex84 -  */
UINT32 SI85; /* Subindex85 -  */
UINT32 SI86; /* Subindex86 -  */
UINT32 SI87; /* Subindex87 -  */
UINT32 SI88; /* Subindex88 -  */
UINT32 SI89; /* Subindex89 -  */
UINT32 SI90; /* Subindex90 -  */
UINT32 SI91; /* Subindex91 -  */
UINT32 SI92; /* Subindex92 -  */
UINT32 SI93; /* Subindex93 -  */
UINT32 SI94; /* Subindex94 -  */
UINT32 SI95; /* Subindex95 -  */
UINT32 SI96; /* Subindex96 -  */
UINT32 SI97; /* Subindex97 -  */
UINT32 SI98; /* Subindex98 -  */
UINT32 SI99; /* Subindex99 -  */
UINT32 SI100; /* Subindex100 -  */
UINT32 SI101; /* Subindex101 -  */
UINT32 SI102; /* Subindex102 -  */
UINT32 SI103; /* Subindex103 -  */
UINT32 SI104; /* Subindex104 -  */
UINT32 SI105; /* Subindex105 -  */
UINT32 SI106; /* Subindex106 -  */
UINT32 SI107; /* Subindex107 -  */
UINT32 SI108; /* Subindex108 -  */
UINT32 SI109; /* Subindex109 -  */
UINT32 SI110; /* Subindex110 -  */
UINT32 SI111; /* Subindex111 -  */
UINT32 SI112; /* Subindex112 -  */
UINT32 SI113; /* Subindex113 -  */
UINT32 SI114; /* Subindex114 -  */
UINT32 SI115; /* Subindex115 -  */
UINT32 SI116; /* Subindex116 -  */
UINT32 SI117; /* Subindex117 -  */
UINT32 SI118; /* Subindex118 -  */
UINT32 SI119; /* Subindex119 -  */
UINT32 SI120; /* Subindex120 -  */
UINT32 SI121; /* Subindex121 -  */
UINT32 SI122; /* Subindex122 -  */
UINT32 SI123; /* Subindex123 -  */
UINT32 SI124; /* Subindex124 -  */
UINT32 SI125; /* Subindex125 -  */
UINT32 SI126; /* Subindex126 -  */
UINT32 SI127; /* Subindex127 -  */
UINT32 SI128; /* Subindex128 -  */
UINT32 SI129; /* Subindex129 -  */
UINT32 SI130; /* Subindex130 -  */
UINT32 SI131; /* Subindex131 -  */
UINT32 SI132; /* Subindex132 -  */
UINT32 SI133; /* Subindex133 -  */
UINT32 SI134; /* Subindex134 -  */
UINT32 SI135; /* Subindex135 -  */
UINT32 SI136; /* Subindex136 -  */
UINT32 SI137; /* Subindex137 -  */
UINT32 SI138; /* Subindex138 -  */
UINT32 SI139; /* Subindex139 -  */
UINT32 SI140; /* Subindex140 -  */
UINT32 SI141; /* Subindex141 -  */
UINT32 SI142; /* Subindex142 -  */
UINT32 SI143; /* Subindex143 -  */
UINT32 SI144; /* Subindex144 -  */
UINT32 SI145; /* Subindex145 -  */
UINT32 SI146; /* Subindex146 -  */
UINT32 SI147; /* Subindex147 -  */
UINT32 SI148; /* Subindex148 -  */
UINT32 SI149; /* Subindex149 -  */
UINT32 SI150; /* Subindex150 -  */
UINT32 SI151; /* Subindex151 -  */
UINT32 SI152; /* Subindex152 -  */
UINT32 SI153; /* Subindex153 -  */
UINT32 SI154; /* Subindex154 -  */
UINT32 SI155; /* Subindex155 -  */
UINT32 SI156; /* Subindex156 -  */
UINT32 SI157; /* Subindex157 -  */
UINT32 SI158; /* Subindex158 -  */
UINT32 SI159; /* Subindex159 -  */
UINT32 SI160; /* Subindex160 -  */
UINT32 SI161; /* Subindex161 -  */
UINT32 SI162; /* Subindex162 -  */
UINT32 SI163; /* Subindex163 -  */
UINT32 SI164; /* Subindex164 -  */
UINT32 SI165; /* Subindex165 -  */
UINT32 SI166; /* Subindex166 -  */
UINT32 SI167; /* Subindex167 -  */
UINT32 SI168; /* Subindex168 -  */
UINT32 SI169; /* Subindex169 -  */
UINT32 SI170; /* Subindex170 -  */
UINT32 SI171; /* Subindex171 -  */
UINT32 SI172; /* Subindex172 -  */
UINT32 SI173; /* Subindex173 -  */
UINT32 SI174; /* Subindex174 -  */
UINT32 SI175; /* Subindex175 -  */
UINT32 SI176; /* Subindex176 -  */
UINT32 SI177; /* Subindex177 -  */
UINT32 SI178; /* Subindex178 -  */
UINT32 SI179; /* Subindex179 -  */
UINT32 SI180; /* Subindex180 -  */
UINT32 SI181; /* Subindex181 -  */
UINT32 SI182; /* Subindex182 -  */
UINT32 SI183; /* Subindex183 -  */
UINT32 SI184; /* Subindex184 -  */
UINT32 SI185; /* Subindex185 -  */
UINT32 SI186; /* Subindex186 -  */
UINT32 SI187; /* Subindex187 -  */
UINT32 SI188; /* Subindex188 -  */
UINT32 SI189; /* Subindex189 -  */
UINT32 SI190; /* Subindex190 -  */
UINT32 SI191; /* Subindex191 -  */
UINT32 SI192; /* Subindex192 -  */
UINT32 SI193; /* Subindex193 -  */
UINT32 SI194; /* Subindex194 -  */
UINT32 SI195; /* Subindex195 -  */
UINT32 SI196; /* Subindex196 -  */
UINT32 SI197; /* Subindex197 -  */
UINT32 SI198; /* Subindex198 -  */
UINT32 SI199; /* Subindex199 -  */
UINT32 SI200; /* Subindex200 -  */
UINT32 SI201; /* Subindex201 -  */
UINT32 SI202; /* Subindex202 -  */
UINT32 SI203; /* Subindex203 -  */
UINT32 SI204; /* Subindex204 -  */
UINT32 SI205; /* Subindex205 -  */
UINT32 SI206; /* Subindex206 -  */
UINT32 SI207; /* Subindex207 -  */
UINT32 SI208; /* Subindex208 -  */
UINT32 SI209; /* Subindex209 -  */
UINT32 SI210; /* Subindex210 -  */
UINT32 SI211; /* Subindex211 -  */
UINT32 SI212; /* Subindex212 -  */
UINT32 SI213; /* Subindex213 -  */
UINT32 SI214; /* Subindex214 -  */
UINT32 SI215; /* Subindex215 -  */
UINT32 SI216; /* Subindex216 -  */
UINT32 SI217; /* Subindex217 -  */
UINT32 SI218; /* Subindex218 -  */
UINT32 SI219; /* Subindex219 -  */
UINT32 SI220; /* Subindex220 -  */
UINT32 SI221; /* Subindex221 -  */
UINT32 SI222; /* Subindex222 -  */
UINT32 SI223; /* Subindex223 -  */
UINT32 SI224; /* Subindex224 -  */
UINT32 SI225; /* Subindex225 -  */
UINT32 SI226; /* Subindex226 -  */
UINT32 SI227; /* Subindex227 -  */
UINT32 SI228; /* Subindex228 -  */
UINT32 SI229; /* Subindex229 -  */
UINT32 SI230; /* Subindex230 -  */
UINT32 SI231; /* Subindex231 -  */
UINT32 SI232; /* Subindex232 -  */
UINT32 SI233; /* Subindex233 -  */
UINT32 SI234; /* Subindex234 -  */
UINT32 SI235; /* Subindex235 -  */
UINT32 SI236; /* Subindex236 -  */
UINT32 SI237; /* Subindex237 -  */
UINT32 SI238; /* Subindex238 -  */
UINT32 SI239; /* Subindex239 -  */
UINT32 SI240; /* Subindex240 -  */
UINT32 SI241; /* Subindex241 -  */
UINT32 SI242; /* Subindex242 -  */
UINT32 SI243; /* Subindex243 -  */
UINT32 SI244; /* Subindex244 -  */
UINT32 SI245; /* Subindex245 -  */
UINT32 SI246; /* Subindex246 -  */
UINT32 SI247; /* Subindex247 -  */
UINT32 SI248; /* Subindex248 -  */
UINT32 SI249; /* Subindex249 -  */
UINT32 SI250; /* Subindex250 -  */
UINT32 SI251; /* Subindex251 -  */
UINT32 SI252; /* Subindex252 -  */
UINT32 SI253; /* Subindex253 -  */
UINT32 SI254; /* Subindex254 -  */
UINT32 SI255; /* Subindex255 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A00;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A00 DIUnit10x1A00
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={255,0x60000110,0x60000210,0x60000310,0x60000410,0x60000510,0x60000610,0x60000710,0x60000810,0x60000910,0x60000A10,0x60000B10,0x60000C10,0x60000D10,0x60000E10,0x60000F10,0x60001010,0x60001110,0x60001210,0x60001310,0x60001410,0x60001510,0x60001610,0x60001710,0x60001810,0x60001910,0x60001A10,0x60001B10,0x60001C10,0x60001D10,0x60001E10,0x60001F10,0x60002010,0x60002110,0x60002210,0x60002310,0x60002410,0x60002510,0x60002610,0x60002710,0x60002810,0x60002910,0x60002A10,0x60002B10,0x60002C10,0x60002D10,0x60002E10,0x60002F10,0x60003010,0x60003110,0x60003210,0x60003310,0x60003410,0x60003510,0x60003610,0x60003710,0x60003810,0x60003910,0x60003A10,0x60003B10,0x60003C10,0x60003D10,0x60003E10,0x60003F10,0x60004010,0x60004110,0x60004210,0x60004310,0x60004410,0x60004510,0x60004610,0x60004710,0x60004810,0x60004910,0x60004A10,0x60004B10,0x60004C10,0x60004D10,0x60004E10,0x60004F10,0x60005010,0x60005110,0x60005210,0x60005310,0x60005410,0x60005510,0x60005610,0x60005710,0x60005810,0x60005910,0x60005A10,0x60005B10,0x60005C10,0x60005D10,0x60005E10,0x60005F10,0x60006010,0x60006110,0x60006210,0x60006310,0x60006410,0x60006510,0x60006610,0x60006710,0x60006810,0x60006910,0x60006A10,0x60006B10,0x60006C10,0x60006D10,0x60006E10,0x60006F10,0x60007010,0x60007110,0x60007210,0x60007310,0x60007410,0x60007510,0x60007610,0x60007710,0x60007810,0x60007910,0x60007A10,0x60007B10,0x60007C10,0x60007D10,0x60007E10,0x60007F10,0x60008010,0x60008110,0x60008210,0x60008310,0x60008410,0x60008510,0x60008610,0x60008710,0x60008810,0x60008910,0x60008A10,0x60008B10,0x60008C10,0x60008D10,0x60008E10,0x60008F10,0x60009010,0x60009110,0x60009210,0x60009310,0x60009410,0x60009510,0x60009610,0x60009710,0x60009810,0x60009910,0x60009A10,0x60009B10,0x60009C10,0x60009D10,0x60009E10,0x60009F10,0x6000A010,0x6000A110,0x6000A210,0x6000A310,0x6000A410,0x6000A510,0x6000A610,0x6000A710,0x6000A810,0x6000A910,0x6000AA10,0x6000AB10,0x6000AC10,0x6000AD10,0x6000AE10,0x6000AF10,0x6000B010,0x6000B110,0x6000B210,0x6000B310,0x6000B410,0x6000B510,0x6000B610,0x6000B710,0x6000B810,0x6000B910,0x6000BA10,0x6000BB10,0x6000BC10,0x6000BD10,0x6000BE10,0x6000BF10,0x6000C010,0x6000C110,0x6000C210,0x6000C310,0x6000C410,0x6000C510,0x6000C610,0x6000C710,0x6000C810,0x6000C910,0x6000CA10,0x6000CB10,0x6000CC10,0x6000CD10,0x6000CE10,0x6000CF10,0x6000D010,0x6000D110,0x6000D210,0x6000D310,0x6000D410,0x6000D510,0x6000D610,0x6000D710,0x6000D810,0x6000D910,0x6000DA10,0x6000DB10,0x6000DC10,0x6000DD10,0x6000DE10,0x6000DF10,0x6000E010,0x6000E110,0x6000E210,0x6000E310,0x6000E410,0x6000E510,0x6000E610,0x6000E710,0x6000E810,0x6000E910,0x6000EA10,0x6000EB10,0x6000EC10,0x6000ED10,0x6000EE10,0x6000EF10,0x6000F010,0x6000F110,0x6000F210,0x6000F310,0x6000F410,0x6000F510,0x6000F610,0x6000F710,0x6000F810,0x6000F910,0x6000FA10,0x6000FB10,0x6000FC10,0x6000FD10,0x6000FE10,0x6000FF10}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1A01 : DI Unit3
******************************************************************************/
/**
* \addtogroup 0x1A01 0x1A01 | DI Unit3
* @{
* \brief Object 0x1A01 (DI Unit3) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1<br>
* SubIndex 2<br>
* SubIndex 3<br>
* SubIndex 4<br>
* SubIndex 5<br>
* SubIndex 6<br>
* SubIndex 7<br>
* SubIndex 8<br>
* SubIndex 9<br>
* SubIndex 10<br>
* SubIndex 11<br>
* SubIndex 12<br>
* SubIndex 13<br>
* SubIndex 14<br>
* SubIndex 15<br>
* SubIndex 16<br>
* SubIndex 17<br>
* SubIndex 18<br>
* SubIndex 19<br>
* SubIndex 20<br>
* SubIndex 21<br>
* SubIndex 22<br>
* SubIndex 23<br>
* SubIndex 24<br>
* SubIndex 25<br>
* SubIndex 26<br>
* SubIndex 27<br>
* SubIndex 28<br>
* SubIndex 29<br>
* SubIndex 30<br>
* SubIndex 31<br>
* SubIndex 32<br>
* SubIndex 33<br>
* SubIndex 34<br>
* SubIndex 35<br>
* SubIndex 36<br>
* SubIndex 37<br>
* SubIndex 38<br>
* SubIndex 39<br>
* SubIndex 40<br>
* SubIndex 41<br>
* SubIndex 42<br>
* SubIndex 43<br>
* SubIndex 44<br>
* SubIndex 45<br>
* SubIndex 46<br>
* SubIndex 47<br>
* SubIndex 48<br>
* SubIndex 49<br>
* SubIndex 50<br>
* SubIndex 51<br>
* SubIndex 52<br>
* SubIndex 53<br>
* SubIndex 54<br>
* SubIndex 55<br>
* SubIndex 56<br>
* SubIndex 57<br>
* SubIndex 58<br>
* SubIndex 59<br>
* SubIndex 60<br>
* SubIndex 61<br>
* SubIndex 62<br>
* SubIndex 63<br>
* SubIndex 64<br>
* SubIndex 65<br>
* SubIndex 66<br>
* SubIndex 67<br>
* SubIndex 68<br>
* SubIndex 69<br>
* SubIndex 70<br>
* SubIndex 71<br>
* SubIndex 72<br>
* SubIndex 73<br>
* SubIndex 74<br>
* SubIndex 75<br>
* SubIndex 76<br>
* SubIndex 77<br>
* SubIndex 78<br>
* SubIndex 79<br>
* SubIndex 80<br>
* SubIndex 81<br>
* SubIndex 82<br>
* SubIndex 83<br>
* SubIndex 84<br>
* SubIndex 85<br>
* SubIndex 86<br>
* SubIndex 87<br>
* SubIndex 88<br>
* SubIndex 89<br>
* SubIndex 90<br>
* SubIndex 91<br>
* SubIndex 92<br>
* SubIndex 93<br>
* SubIndex 94<br>
* SubIndex 95<br>
* SubIndex 96<br>
* SubIndex 97<br>
* SubIndex 98<br>
* SubIndex 99<br>
* SubIndex 100<br>
* SubIndex 101<br>
* SubIndex 102<br>
* SubIndex 103<br>
* SubIndex 104<br>
* SubIndex 105<br>
* SubIndex 106<br>
* SubIndex 107<br>
* SubIndex 108<br>
* SubIndex 109<br>
* SubIndex 110<br>
* SubIndex 111<br>
* SubIndex 112<br>
* SubIndex 113<br>
* SubIndex 114<br>
* SubIndex 115<br>
* SubIndex 116<br>
* SubIndex 117<br>
* SubIndex 118<br>
* SubIndex 119<br>
* SubIndex 120<br>
* SubIndex 121<br>
* SubIndex 122<br>
* SubIndex 123<br>
* SubIndex 124<br>
* SubIndex 125<br>
* SubIndex 126<br>
* SubIndex 127<br>
* SubIndex 128<br>
* SubIndex 129<br>
* SubIndex 130<br>
* SubIndex 131<br>
* SubIndex 132<br>
* SubIndex 133<br>
* SubIndex 134<br>
* SubIndex 135<br>
* SubIndex 136<br>
* SubIndex 137<br>
* SubIndex 138<br>
* SubIndex 139<br>
* SubIndex 140<br>
* SubIndex 141<br>
* SubIndex 142<br>
* SubIndex 143<br>
* SubIndex 144<br>
* SubIndex 145<br>
* SubIndex 146<br>
* SubIndex 147<br>
* SubIndex 148<br>
* SubIndex 149<br>
* SubIndex 150<br>
* SubIndex 151<br>
* SubIndex 152<br>
* SubIndex 153<br>
* SubIndex 154<br>
* SubIndex 155<br>
* SubIndex 156<br>
* SubIndex 157<br>
* SubIndex 158<br>
* SubIndex 159<br>
* SubIndex 160<br>
* SubIndex 161<br>
* SubIndex 162<br>
* SubIndex 163<br>
* SubIndex 164<br>
* SubIndex 165<br>
* SubIndex 166<br>
* SubIndex 167<br>
* SubIndex 168<br>
* SubIndex 169<br>
* SubIndex 170<br>
* SubIndex 171<br>
* SubIndex 172<br>
* SubIndex 173<br>
* SubIndex 174<br>
* SubIndex 175<br>
* SubIndex 176<br>
* SubIndex 177<br>
* SubIndex 178<br>
* SubIndex 179<br>
* SubIndex 180<br>
* SubIndex 181<br>
* SubIndex 182<br>
* SubIndex 183<br>
* SubIndex 184<br>
* SubIndex 185<br>
* SubIndex 186<br>
* SubIndex 187<br>
* SubIndex 188<br>
* SubIndex 189<br>
* SubIndex 190<br>
* SubIndex 191<br>
* SubIndex 192<br>
* SubIndex 193<br>
* SubIndex 194<br>
* SubIndex 195<br>
* SubIndex 196<br>
* SubIndex 197<br>
* SubIndex 198<br>
* SubIndex 199<br>
* SubIndex 200<br>
* SubIndex 201<br>
* SubIndex 202<br>
* SubIndex 203<br>
* SubIndex 204<br>
* SubIndex 205<br>
* SubIndex 206<br>
* SubIndex 207<br>
* SubIndex 208<br>
* SubIndex 209<br>
* SubIndex 210<br>
* SubIndex 211<br>
* SubIndex 212<br>
* SubIndex 213<br>
* SubIndex 214<br>
* SubIndex 215<br>
* SubIndex 216<br>
* SubIndex 217<br>
* SubIndex 218<br>
* SubIndex 219<br>
* SubIndex 220<br>
* SubIndex 221<br>
* SubIndex 222<br>
* SubIndex 223<br>
* SubIndex 224<br>
* SubIndex 225<br>
* SubIndex 226<br>
* SubIndex 227<br>
* SubIndex 228<br>
* SubIndex 229<br>
* SubIndex 230<br>
* SubIndex 231<br>
* SubIndex 232<br>
* SubIndex 233<br>
* SubIndex 234<br>
* SubIndex 235<br>
* SubIndex 236<br>
* SubIndex 237<br>
* SubIndex 238<br>
* SubIndex 239<br>
* SubIndex 240<br>
* SubIndex 241<br>
* SubIndex 242<br>
* SubIndex 243<br>
* SubIndex 244<br>
* SubIndex 245<br>
* SubIndex 246<br>
* SubIndex 247<br>
* SubIndex 248<br>
* SubIndex 249<br>
* SubIndex 250<br>
* SubIndex 251<br>
* SubIndex 252<br>
* SubIndex 253<br>
* SubIndex 254<br>
* SubIndex 255<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1A01[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex1 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex2 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex3 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex4 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex5 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex6 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex7 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex8 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex9 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex10 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex11 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex12 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex13 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex14 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex15 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex16 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex17 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex18 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex19 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex20 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex21 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex22 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex23 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex24 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex25 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex26 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex27 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex28 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex29 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex30 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex31 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex32 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex33 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex34 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex35 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex36 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex37 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex38 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex39 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex40 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex41 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex42 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex43 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex44 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex45 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex46 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex47 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex48 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex49 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex50 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex51 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex52 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex53 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex54 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex55 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex56 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex57 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex58 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex59 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex60 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex61 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex62 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex63 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex64 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex65 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex66 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex67 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex68 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex69 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex70 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex71 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex72 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex73 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex74 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex75 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex76 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex77 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex78 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex79 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex80 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex81 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex82 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex83 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex84 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex85 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex86 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex87 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex88 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex89 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex90 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex91 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex92 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex93 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex94 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex95 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex96 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex97 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex98 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex99 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex100 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex101 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex102 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex103 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex104 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex105 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex106 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex107 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex108 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex109 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex110 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex111 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex112 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex113 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex114 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex115 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex116 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex117 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex118 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex119 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex120 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex121 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex122 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex123 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex124 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex125 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex126 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex127 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex128 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex129 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex130 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex131 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex132 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex133 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex134 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex135 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex136 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex137 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex138 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex139 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex140 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex141 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex142 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex143 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex144 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex145 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex146 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex147 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex148 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex149 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex150 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex151 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex152 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex153 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex154 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex155 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex156 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex157 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex158 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex159 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex160 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex161 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex162 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex163 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex164 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex165 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex166 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex167 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex168 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex169 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex170 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex171 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex172 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex173 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex174 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex175 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex176 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex177 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex178 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex179 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex180 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex181 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex182 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex183 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex184 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex185 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex186 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex187 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex188 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex189 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex190 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex191 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex192 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex193 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex194 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex195 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex196 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex197 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex198 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex199 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex200 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex201 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex202 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex203 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex204 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex205 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex206 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex207 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex208 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex209 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex210 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex211 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex212 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex213 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex214 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex215 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex216 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex217 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex218 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex219 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex220 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex221 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex222 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex223 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex224 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex225 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex226 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex227 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex228 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex229 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex230 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex231 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex232 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex233 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex234 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex235 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex236 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex237 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex238 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex239 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex240 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex241 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex242 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex243 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex244 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex245 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex246 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex247 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex248 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex249 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex250 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex251 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex252 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex253 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }, /* Subindex254 */
{ DEFTYPE_UNSIGNED32 , 0x20 , ACCESS_READ }}; /* Subindex255 */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0x1A01[] = "DI Unit3\000"
"SubIndex 001\000"
"SubIndex 002\000"
"SubIndex 003\000"
"SubIndex 004\000"
"SubIndex 005\000"
"SubIndex 006\000"
"SubIndex 007\000"
"SubIndex 008\000"
"SubIndex 009\000"
"SubIndex 010\000"
"SubIndex 011\000"
"SubIndex 012\000"
"SubIndex 013\000"
"SubIndex 014\000"
"SubIndex 015\000"
"SubIndex 016\000"
"SubIndex 017\000"
"SubIndex 018\000"
"SubIndex 019\000"
"SubIndex 020\000"
"SubIndex 021\000"
"SubIndex 022\000"
"SubIndex 023\000"
"SubIndex 024\000"
"SubIndex 025\000"
"SubIndex 026\000"
"SubIndex 027\000"
"SubIndex 028\000"
"SubIndex 029\000"
"SubIndex 030\000"
"SubIndex 031\000"
"SubIndex 032\000"
"SubIndex 033\000"
"SubIndex 034\000"
"SubIndex 035\000"
"SubIndex 036\000"
"SubIndex 037\000"
"SubIndex 038\000"
"SubIndex 039\000"
"SubIndex 040\000"
"SubIndex 041\000"
"SubIndex 042\000"
"SubIndex 043\000"
"SubIndex 044\000"
"SubIndex 045\000"
"SubIndex 046\000"
"SubIndex 047\000"
"SubIndex 048\000"
"SubIndex 049\000"
"SubIndex 050\000"
"SubIndex 051\000"
"SubIndex 052\000"
"SubIndex 053\000"
"SubIndex 054\000"
"SubIndex 055\000"
"SubIndex 056\000"
"SubIndex 057\000"
"SubIndex 058\000"
"SubIndex 059\000"
"SubIndex 060\000"
"SubIndex 061\000"
"SubIndex 062\000"
"SubIndex 063\000"
"SubIndex 064\000"
"SubIndex 065\000"
"SubIndex 066\000"
"SubIndex 067\000"
"SubIndex 068\000"
"SubIndex 069\000"
"SubIndex 070\000"
"SubIndex 071\000"
"SubIndex 072\000"
"SubIndex 073\000"
"SubIndex 074\000"
"SubIndex 075\000"
"SubIndex 076\000"
"SubIndex 077\000"
"SubIndex 078\000"
"SubIndex 079\000"
"SubIndex 080\000"
"SubIndex 081\000"
"SubIndex 082\000"
"SubIndex 083\000"
"SubIndex 084\000"
"SubIndex 085\000"
"SubIndex 086\000"
"SubIndex 087\000"
"SubIndex 088\000"
"SubIndex 089\000"
"SubIndex 090\000"
"SubIndex 091\000"
"SubIndex 092\000"
"SubIndex 093\000"
"SubIndex 094\000"
"SubIndex 095\000"
"SubIndex 096\000"
"SubIndex 097\000"
"SubIndex 098\000"
"SubIndex 099\000"
"SubIndex 100\000"
"SubIndex 101\000"
"SubIndex 102\000"
"SubIndex 103\000"
"SubIndex 104\000"
"SubIndex 105\000"
"SubIndex 106\000"
"SubIndex 107\000"
"SubIndex 108\000"
"SubIndex 109\000"
"SubIndex 110\000"
"SubIndex 111\000"
"SubIndex 112\000"
"SubIndex 113\000"
"SubIndex 114\000"
"SubIndex 115\000"
"SubIndex 116\000"
"SubIndex 117\000"
"SubIndex 118\000"
"SubIndex 119\000"
"SubIndex 120\000"
"SubIndex 121\000"
"SubIndex 122\000"
"SubIndex 123\000"
"SubIndex 124\000"
"SubIndex 125\000"
"SubIndex 126\000"
"SubIndex 127\000"
"SubIndex 128\000"
"SubIndex 129\000"
"SubIndex 130\000"
"SubIndex 131\000"
"SubIndex 132\000"
"SubIndex 133\000"
"SubIndex 134\000"
"SubIndex 135\000"
"SubIndex 136\000"
"SubIndex 137\000"
"SubIndex 138\000"
"SubIndex 139\000"
"SubIndex 140\000"
"SubIndex 141\000"
"SubIndex 142\000"
"SubIndex 143\000"
"SubIndex 144\000"
"SubIndex 145\000"
"SubIndex 146\000"
"SubIndex 147\000"
"SubIndex 148\000"
"SubIndex 149\000"
"SubIndex 150\000"
"SubIndex 151\000"
"SubIndex 152\000"
"SubIndex 153\000"
"SubIndex 154\000"
"SubIndex 155\000"
"SubIndex 156\000"
"SubIndex 157\000"
"SubIndex 158\000"
"SubIndex 159\000"
"SubIndex 160\000"
"SubIndex 161\000"
"SubIndex 162\000"
"SubIndex 163\000"
"SubIndex 164\000"
"SubIndex 165\000"
"SubIndex 166\000"
"SubIndex 167\000"
"SubIndex 168\000"
"SubIndex 169\000"
"SubIndex 170\000"
"SubIndex 171\000"
"SubIndex 172\000"
"SubIndex 173\000"
"SubIndex 174\000"
"SubIndex 175\000"
"SubIndex 176\000"
"SubIndex 177\000"
"SubIndex 178\000"
"SubIndex 179\000"
"SubIndex 180\000"
"SubIndex 181\000"
"SubIndex 182\000"
"SubIndex 183\000"
"SubIndex 184\000"
"SubIndex 185\000"
"SubIndex 186\000"
"SubIndex 187\000"
"SubIndex 188\000"
"SubIndex 189\000"
"SubIndex 190\000"
"SubIndex 191\000"
"SubIndex 192\000"
"SubIndex 193\000"
"SubIndex 194\000"
"SubIndex 195\000"
"SubIndex 196\000"
"SubIndex 197\000"
"SubIndex 198\000"
"SubIndex 199\000"
"SubIndex 200\000"
"SubIndex 201\000"
"SubIndex 202\000"
"SubIndex 203\000"
"SubIndex 204\000"
"SubIndex 205\000"
"SubIndex 206\000"
"SubIndex 207\000"
"SubIndex 208\000"
"SubIndex 209\000"
"SubIndex 210\000"
"SubIndex 211\000"
"SubIndex 212\000"
"SubIndex 213\000"
"SubIndex 214\000"
"SubIndex 215\000"
"SubIndex 216\000"
"SubIndex 217\000"
"SubIndex 218\000"
"SubIndex 219\000"
"SubIndex 220\000"
"SubIndex 221\000"
"SubIndex 222\000"
"SubIndex 223\000"
"SubIndex 224\000"
"SubIndex 225\000"
"SubIndex 226\000"
"SubIndex 227\000"
"SubIndex 228\000"
"SubIndex 229\000"
"SubIndex 230\000"
"SubIndex 231\000"
"SubIndex 232\000"
"SubIndex 233\000"
"SubIndex 234\000"
"SubIndex 235\000"
"SubIndex 236\000"
"SubIndex 237\000"
"SubIndex 238\000"
"SubIndex 239\000"
"SubIndex 240\000"
"SubIndex 241\000"
"SubIndex 242\000"
"SubIndex 243\000"
"SubIndex 244\000"
"SubIndex 245\000"
"SubIndex 246\000"
"SubIndex 247\000"
"SubIndex 248\000"
"SubIndex 249\000"
"SubIndex 250\000"
"SubIndex 251\000"
"SubIndex 252\000"
"SubIndex 253\000"
"SubIndex 254\000"
"SubIndex 255\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT32 SI1; /* Subindex1 -  */
UINT32 SI2; /* Subindex2 -  */
UINT32 SI3; /* Subindex3 -  */
UINT32 SI4; /* Subindex4 -  */
UINT32 SI5; /* Subindex5 -  */
UINT32 SI6; /* Subindex6 -  */
UINT32 SI7; /* Subindex7 -  */
UINT32 SI8; /* Subindex8 -  */
UINT32 SI9; /* Subindex9 -  */
UINT32 SI10; /* Subindex10 -  */
UINT32 SI11; /* Subindex11 -  */
UINT32 SI12; /* Subindex12 -  */
UINT32 SI13; /* Subindex13 -  */
UINT32 SI14; /* Subindex14 -  */
UINT32 SI15; /* Subindex15 -  */
UINT32 SI16; /* Subindex16 -  */
UINT32 SI17; /* Subindex17 -  */
UINT32 SI18; /* Subindex18 -  */
UINT32 SI19; /* Subindex19 -  */
UINT32 SI20; /* Subindex20 -  */
UINT32 SI21; /* Subindex21 -  */
UINT32 SI22; /* Subindex22 -  */
UINT32 SI23; /* Subindex23 -  */
UINT32 SI24; /* Subindex24 -  */
UINT32 SI25; /* Subindex25 -  */
UINT32 SI26; /* Subindex26 -  */
UINT32 SI27; /* Subindex27 -  */
UINT32 SI28; /* Subindex28 -  */
UINT32 SI29; /* Subindex29 -  */
UINT32 SI30; /* Subindex30 -  */
UINT32 SI31; /* Subindex31 -  */
UINT32 SI32; /* Subindex32 -  */
UINT32 SI33; /* Subindex33 -  */
UINT32 SI34; /* Subindex34 -  */
UINT32 SI35; /* Subindex35 -  */
UINT32 SI36; /* Subindex36 -  */
UINT32 SI37; /* Subindex37 -  */
UINT32 SI38; /* Subindex38 -  */
UINT32 SI39; /* Subindex39 -  */
UINT32 SI40; /* Subindex40 -  */
UINT32 SI41; /* Subindex41 -  */
UINT32 SI42; /* Subindex42 -  */
UINT32 SI43; /* Subindex43 -  */
UINT32 SI44; /* Subindex44 -  */
UINT32 SI45; /* Subindex45 -  */
UINT32 SI46; /* Subindex46 -  */
UINT32 SI47; /* Subindex47 -  */
UINT32 SI48; /* Subindex48 -  */
UINT32 SI49; /* Subindex49 -  */
UINT32 SI50; /* Subindex50 -  */
UINT32 SI51; /* Subindex51 -  */
UINT32 SI52; /* Subindex52 -  */
UINT32 SI53; /* Subindex53 -  */
UINT32 SI54; /* Subindex54 -  */
UINT32 SI55; /* Subindex55 -  */
UINT32 SI56; /* Subindex56 -  */
UINT32 SI57; /* Subindex57 -  */
UINT32 SI58; /* Subindex58 -  */
UINT32 SI59; /* Subindex59 -  */
UINT32 SI60; /* Subindex60 -  */
UINT32 SI61; /* Subindex61 -  */
UINT32 SI62; /* Subindex62 -  */
UINT32 SI63; /* Subindex63 -  */
UINT32 SI64; /* Subindex64 -  */
UINT32 SI65; /* Subindex65 -  */
UINT32 SI66; /* Subindex66 -  */
UINT32 SI67; /* Subindex67 -  */
UINT32 SI68; /* Subindex68 -  */
UINT32 SI69; /* Subindex69 -  */
UINT32 SI70; /* Subindex70 -  */
UINT32 SI71; /* Subindex71 -  */
UINT32 SI72; /* Subindex72 -  */
UINT32 SI73; /* Subindex73 -  */
UINT32 SI74; /* Subindex74 -  */
UINT32 SI75; /* Subindex75 -  */
UINT32 SI76; /* Subindex76 -  */
UINT32 SI77; /* Subindex77 -  */
UINT32 SI78; /* Subindex78 -  */
UINT32 SI79; /* Subindex79 -  */
UINT32 SI80; /* Subindex80 -  */
UINT32 SI81; /* Subindex81 -  */
UINT32 SI82; /* Subindex82 -  */
UINT32 SI83; /* Subindex83 -  */
UINT32 SI84; /* Subindex84 -  */
UINT32 SI85; /* Subindex85 -  */
UINT32 SI86; /* Subindex86 -  */
UINT32 SI87; /* Subindex87 -  */
UINT32 SI88; /* Subindex88 -  */
UINT32 SI89; /* Subindex89 -  */
UINT32 SI90; /* Subindex90 -  */
UINT32 SI91; /* Subindex91 -  */
UINT32 SI92; /* Subindex92 -  */
UINT32 SI93; /* Subindex93 -  */
UINT32 SI94; /* Subindex94 -  */
UINT32 SI95; /* Subindex95 -  */
UINT32 SI96; /* Subindex96 -  */
UINT32 SI97; /* Subindex97 -  */
UINT32 SI98; /* Subindex98 -  */
UINT32 SI99; /* Subindex99 -  */
UINT32 SI100; /* Subindex100 -  */
UINT32 SI101; /* Subindex101 -  */
UINT32 SI102; /* Subindex102 -  */
UINT32 SI103; /* Subindex103 -  */
UINT32 SI104; /* Subindex104 -  */
UINT32 SI105; /* Subindex105 -  */
UINT32 SI106; /* Subindex106 -  */
UINT32 SI107; /* Subindex107 -  */
UINT32 SI108; /* Subindex108 -  */
UINT32 SI109; /* Subindex109 -  */
UINT32 SI110; /* Subindex110 -  */
UINT32 SI111; /* Subindex111 -  */
UINT32 SI112; /* Subindex112 -  */
UINT32 SI113; /* Subindex113 -  */
UINT32 SI114; /* Subindex114 -  */
UINT32 SI115; /* Subindex115 -  */
UINT32 SI116; /* Subindex116 -  */
UINT32 SI117; /* Subindex117 -  */
UINT32 SI118; /* Subindex118 -  */
UINT32 SI119; /* Subindex119 -  */
UINT32 SI120; /* Subindex120 -  */
UINT32 SI121; /* Subindex121 -  */
UINT32 SI122; /* Subindex122 -  */
UINT32 SI123; /* Subindex123 -  */
UINT32 SI124; /* Subindex124 -  */
UINT32 SI125; /* Subindex125 -  */
UINT32 SI126; /* Subindex126 -  */
UINT32 SI127; /* Subindex127 -  */
UINT32 SI128; /* Subindex128 -  */
UINT32 SI129; /* Subindex129 -  */
UINT32 SI130; /* Subindex130 -  */
UINT32 SI131; /* Subindex131 -  */
UINT32 SI132; /* Subindex132 -  */
UINT32 SI133; /* Subindex133 -  */
UINT32 SI134; /* Subindex134 -  */
UINT32 SI135; /* Subindex135 -  */
UINT32 SI136; /* Subindex136 -  */
UINT32 SI137; /* Subindex137 -  */
UINT32 SI138; /* Subindex138 -  */
UINT32 SI139; /* Subindex139 -  */
UINT32 SI140; /* Subindex140 -  */
UINT32 SI141; /* Subindex141 -  */
UINT32 SI142; /* Subindex142 -  */
UINT32 SI143; /* Subindex143 -  */
UINT32 SI144; /* Subindex144 -  */
UINT32 SI145; /* Subindex145 -  */
UINT32 SI146; /* Subindex146 -  */
UINT32 SI147; /* Subindex147 -  */
UINT32 SI148; /* Subindex148 -  */
UINT32 SI149; /* Subindex149 -  */
UINT32 SI150; /* Subindex150 -  */
UINT32 SI151; /* Subindex151 -  */
UINT32 SI152; /* Subindex152 -  */
UINT32 SI153; /* Subindex153 -  */
UINT32 SI154; /* Subindex154 -  */
UINT32 SI155; /* Subindex155 -  */
UINT32 SI156; /* Subindex156 -  */
UINT32 SI157; /* Subindex157 -  */
UINT32 SI158; /* Subindex158 -  */
UINT32 SI159; /* Subindex159 -  */
UINT32 SI160; /* Subindex160 -  */
UINT32 SI161; /* Subindex161 -  */
UINT32 SI162; /* Subindex162 -  */
UINT32 SI163; /* Subindex163 -  */
UINT32 SI164; /* Subindex164 -  */
UINT32 SI165; /* Subindex165 -  */
UINT32 SI166; /* Subindex166 -  */
UINT32 SI167; /* Subindex167 -  */
UINT32 SI168; /* Subindex168 -  */
UINT32 SI169; /* Subindex169 -  */
UINT32 SI170; /* Subindex170 -  */
UINT32 SI171; /* Subindex171 -  */
UINT32 SI172; /* Subindex172 -  */
UINT32 SI173; /* Subindex173 -  */
UINT32 SI174; /* Subindex174 -  */
UINT32 SI175; /* Subindex175 -  */
UINT32 SI176; /* Subindex176 -  */
UINT32 SI177; /* Subindex177 -  */
UINT32 SI178; /* Subindex178 -  */
UINT32 SI179; /* Subindex179 -  */
UINT32 SI180; /* Subindex180 -  */
UINT32 SI181; /* Subindex181 -  */
UINT32 SI182; /* Subindex182 -  */
UINT32 SI183; /* Subindex183 -  */
UINT32 SI184; /* Subindex184 -  */
UINT32 SI185; /* Subindex185 -  */
UINT32 SI186; /* Subindex186 -  */
UINT32 SI187; /* Subindex187 -  */
UINT32 SI188; /* Subindex188 -  */
UINT32 SI189; /* Subindex189 -  */
UINT32 SI190; /* Subindex190 -  */
UINT32 SI191; /* Subindex191 -  */
UINT32 SI192; /* Subindex192 -  */
UINT32 SI193; /* Subindex193 -  */
UINT32 SI194; /* Subindex194 -  */
UINT32 SI195; /* Subindex195 -  */
UINT32 SI196; /* Subindex196 -  */
UINT32 SI197; /* Subindex197 -  */
UINT32 SI198; /* Subindex198 -  */
UINT32 SI199; /* Subindex199 -  */
UINT32 SI200; /* Subindex200 -  */
UINT32 SI201; /* Subindex201 -  */
UINT32 SI202; /* Subindex202 -  */
UINT32 SI203; /* Subindex203 -  */
UINT32 SI204; /* Subindex204 -  */
UINT32 SI205; /* Subindex205 -  */
UINT32 SI206; /* Subindex206 -  */
UINT32 SI207; /* Subindex207 -  */
UINT32 SI208; /* Subindex208 -  */
UINT32 SI209; /* Subindex209 -  */
UINT32 SI210; /* Subindex210 -  */
UINT32 SI211; /* Subindex211 -  */
UINT32 SI212; /* Subindex212 -  */
UINT32 SI213; /* Subindex213 -  */
UINT32 SI214; /* Subindex214 -  */
UINT32 SI215; /* Subindex215 -  */
UINT32 SI216; /* Subindex216 -  */
UINT32 SI217; /* Subindex217 -  */
UINT32 SI218; /* Subindex218 -  */
UINT32 SI219; /* Subindex219 -  */
UINT32 SI220; /* Subindex220 -  */
UINT32 SI221; /* Subindex221 -  */
UINT32 SI222; /* Subindex222 -  */
UINT32 SI223; /* Subindex223 -  */
UINT32 SI224; /* Subindex224 -  */
UINT32 SI225; /* Subindex225 -  */
UINT32 SI226; /* Subindex226 -  */
UINT32 SI227; /* Subindex227 -  */
UINT32 SI228; /* Subindex228 -  */
UINT32 SI229; /* Subindex229 -  */
UINT32 SI230; /* Subindex230 -  */
UINT32 SI231; /* Subindex231 -  */
UINT32 SI232; /* Subindex232 -  */
UINT32 SI233; /* Subindex233 -  */
UINT32 SI234; /* Subindex234 -  */
UINT32 SI235; /* Subindex235 -  */
UINT32 SI236; /* Subindex236 -  */
UINT32 SI237; /* Subindex237 -  */
UINT32 SI238; /* Subindex238 -  */
UINT32 SI239; /* Subindex239 -  */
UINT32 SI240; /* Subindex240 -  */
UINT32 SI241; /* Subindex241 -  */
UINT32 SI242; /* Subindex242 -  */
UINT32 SI243; /* Subindex243 -  */
UINT32 SI244; /* Subindex244 -  */
UINT32 SI245; /* Subindex245 -  */
UINT32 SI246; /* Subindex246 -  */
UINT32 SI247; /* Subindex247 -  */
UINT32 SI248; /* Subindex248 -  */
UINT32 SI249; /* Subindex249 -  */
UINT32 SI250; /* Subindex250 -  */
UINT32 SI251; /* Subindex251 -  */
UINT32 SI252; /* Subindex252 -  */
UINT32 SI253; /* Subindex253 -  */
UINT32 SI254; /* Subindex254 -  */
UINT32 SI255; /* Subindex255 -  */
} OBJ_STRUCT_PACKED_END
TOBJ1A01;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1A01 DIUnit30x1A01
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={255,0x60010110,0x60010210,0x60010310,0x60010410,0x60010510,0x60010610,0x60010710,0x60010810,0x60010910,0x60010A10,0x60010B10,0x60010C10,0x60010D10,0x60010E10,0x60010F10,0x60011010,0x60011110,0x60011210,0x60011310,0x60011410,0x60011510,0x60011610,0x60011710,0x60011810,0x60011910,0x60011A10,0x60011B10,0x60011C10,0x60011D10,0x60011E10,0x60011F10,0x60012010,0x60012110,0x60012210,0x60012310,0x60012410,0x60012510,0x60012610,0x60012710,0x60012810,0x60012910,0x60012A10,0x60012B10,0x60012C10,0x60012D10,0x60012E10,0x60012F10,0x60013010,0x60013110,0x60013210,0x60013310,0x60013410,0x60013510,0x60013610,0x60013710,0x60013810,0x60013910,0x60013A10,0x60013B10,0x60013C10,0x60013D10,0x60013E10,0x60013F10,0x60014010,0x60014110,0x60014210,0x60014310,0x60014410,0x60014510,0x60014610,0x60014710,0x60014810,0x60014910,0x60014A10,0x60014B10,0x60014C10,0x60014D10,0x60014E10,0x60014F10,0x60015010,0x60015110,0x60015210,0x60015310,0x60015410,0x60015510,0x60015610,0x60015710,0x60015810,0x60015910,0x60015A10,0x60015B10,0x60015C10,0x60015D10,0x60015E10,0x60015F10,0x60016010,0x60016110,0x60016210,0x60016310,0x60016410,0x60016510,0x60016610,0x60016710,0x60016810,0x60016910,0x60016A10,0x60016B10,0x60016C10,0x60016D10,0x60016E10,0x60016F10,0x60017010,0x60017110,0x60017210,0x60017310,0x60017410,0x60017510,0x60017610,0x60017710,0x60017810,0x60017910,0x60017A10,0x60017B10,0x60017C10,0x60017D10,0x60017E10,0x60017F10,0x60018010,0x60018110,0x60018210,0x60018310,0x60018410,0x60018510,0x60018610,0x60018710,0x60018810,0x60018910,0x60018A10,0x60018B10,0x60018C10,0x60018D10,0x60018E10,0x60018F10,0x60019010,0x60019110,0x60019210,0x60019310,0x60019410,0x60019510,0x60019610,0x60019710,0x60019810,0x60019910,0x60019A10,0x60019B10,0x60019C10,0x60019D10,0x60019E10,0x60019F10,0x6001A010,0x6001A110,0x6001A210,0x6001A310,0x6001A410,0x6001A510,0x6001A610,0x6001A710,0x6001A810,0x6001A910,0x6001AA10,0x6001AB10,0x6001AC10,0x6001AD10,0x6001AE10,0x6001AF10,0x6001B010,0x6001B110,0x6001B210,0x6001B310,0x6001B410,0x6001B510,0x6001B610,0x6001B710,0x6001B810,0x6001B910,0x6001BA10,0x6001BB10,0x6001BC10,0x6001BD10,0x6001BE10,0x6001BF10,0x6001C010,0x6001C110,0x6001C210,0x6001C310,0x6001C410,0x6001C510,0x6001C610,0x6001C710,0x6001C810,0x6001C910,0x6001CA10,0x6001CB10,0x6001CC10,0x6001CD10,0x6001CE10,0x6001CF10,0x6001D010,0x6001D110,0x6001D210,0x6001D310,0x6001D410,0x6001D510,0x6001D610,0x6001D710,0x6001D810,0x6001D910,0x6001DA10,0x6001DB10,0x6001DC10,0x6001DD10,0x6001DE10,0x6001DF10,0x6001E010,0x6001E110,0x6001E210,0x6001E310,0x6001E410,0x6001E510,0x6001E610,0x6001E710,0x6001E810,0x6001E910,0x6001EA10,0x6001EB10,0x6001EC10,0x6001ED10,0x6001EE10,0x6001EF10,0x6001F010,0x6001F110,0x6001F210,0x6001F310,0x6001F410,0x6001F510,0x6001F610,0x6001F710,0x6001F810,0x6001F910,0x6001FA10,0x6001FB10,0x6001FC10,0x6001FD10,0x6001FE10,0x6001FF10}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C12 : SyncManager 2 assignment
******************************************************************************/
/**
* \addtogroup 0x1C12 0x1C12 | SyncManager 2 assignment
* @{
* \brief Object 0x1C12 (SyncManager 2 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C12[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C12[] = "SyncManager 2 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[2];  /**< \brief Subindex 1 - 2 */
} OBJ_STRUCT_PACKED_END
TOBJ1C12;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C12 sRxPDOassign
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={2,{0x1600,0x1601}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x1C13 : SyncManager 3 assignment
******************************************************************************/
/**
* \addtogroup 0x1C13 0x1C13 | SyncManager 3 assignment
* @{
* \brief Object 0x1C13 (SyncManager 3 assignment) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C13[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x1C13[] = "SyncManager 3 assignment\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[2];  /**< \brief Subindex 1 - 2 */
} OBJ_STRUCT_PACKED_END
TOBJ1C13;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ1C13 sTxPDOassign
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={2,{0x1A00,0x1A01}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6000 : DI Unit1
******************************************************************************/
/**
* \addtogroup 0x6000 0x6000 | DI Unit1
* @{
* \brief Object 0x6000 (DI Unit1) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6000[] = "DI Unit1\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[255];  /**< \brief Subindex 1 - 255 */
} OBJ_STRUCT_PACKED_END
TOBJ6000;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6000 DIUnit10x6000
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={255,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x6001 : DI Unit3
******************************************************************************/
/**
* \addtogroup 0x6001 0x6001 | DI Unit3
* @{
* \brief Object 0x6001 (DI Unit3) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x6001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x6001[] = "DI Unit3\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[255];  /**< \brief Subindex 1 - 255 */
} OBJ_STRUCT_PACKED_END
TOBJ6001;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ6001 DIUnit30x6001
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={255,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7000 : DO Unit2
******************************************************************************/
/**
* \addtogroup 0x7000 0x7000 | DO Unit2
* @{
* \brief Object 0x7000 (DO Unit2) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7000[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7000[] = "DO Unit2\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[255];  /**< \brief Subindex 1 - 255 */
} OBJ_STRUCT_PACKED_END
TOBJ7000;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7000 DOUnit20x7000
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={255,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0x7001 : DO Unit4
******************************************************************************/
/**
* \addtogroup 0x7001 0x7001 | DO Unit4
* @{
* \brief Object 0x7001 (DO Unit4) definition
*/
#ifdef _OBJD_
/**
* \brief Entry descriptions<br>
* 
* Subindex 0<br>
* Subindex 1 - n (the same entry description is used)<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x7001[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }};

/**
* \brief Object name definition<br>
* For Subindex 1 to n the syntax 'Subindex XXX' is used
*/
OBJCONST UCHAR OBJMEM aName0x7001[] = "DO Unit4\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16   u16SubIndex0;  /**< \brief Subindex 0 */
UINT16 aEntries[255];  /**< \brief Subindex 1 - 255 */
} OBJ_STRUCT_PACKED_END
TOBJ7001;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJ7001 DOUnit40x7001
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={255,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}}
#endif
;
/** @}*/



/******************************************************************************
*                    Object 0xF008 : Parameters Handing
******************************************************************************/
/**
* \addtogroup 0xF008 0xF008 | Parameters Handing
* @{
* \brief Object 0xF008 (Parameters Handing) definition
*/
#ifdef _OBJD_
/**
* \brief Object entry descriptions<br>
* <br>
* SubIndex 0<br>
* SubIndex 1 - Save Current Parameters<br>
* SubIndex 2 - Restore Default Parameters<br>
* SubIndex 3 - Checksum<br>
* SubIndex 4 - Vendor Reserve<br>
*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0xF008[] = {
{ DEFTYPE_UNSIGNED8 , 0x8 , ACCESS_READ },
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE }, /* Subindex1 - Save Current Parameters */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE }, /* Subindex2 - Restore Default Parameters */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READ }, /* Subindex3 - Checksum */
{ DEFTYPE_UNSIGNED16 , 0x10 , ACCESS_READWRITE }}; /* Subindex4 - Vendor Reserve */

/**
* \brief Object/Entry names
*/
OBJCONST UCHAR OBJMEM aName0xF008[] = "Parameters Handing\000"
"Save Current Parameters\000"
"Restore Default Parameters\000"
"Checksum\000"
"Vendor Reserve\000\377";
#endif //#ifdef _OBJD_

#ifndef _SSC_DEVICE_OBJECTS_H_
/**
* \brief Object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
UINT16 u16SubIndex0;
UINT16 SaveCurrentParameters; /* Subindex1 - Save Current Parameters */
UINT16 RestoreDefaultParameters; /* Subindex2 - Restore Default Parameters */
UINT16 Checksum; /* Subindex3 - Checksum */
UINT16 VendorReserve; /* Subindex4 - Vendor Reserve */
} OBJ_STRUCT_PACKED_END
TOBJF008;
#endif //#ifndef _SSC_DEVICE_OBJECTS_H_

/**
* \brief Object variable
*/
PROTO TOBJF008 ParametersHanding0xF008
#if defined(_SSC_DEVICE_) && (_SSC_DEVICE_ == 1)
={4,0x0000,0x0000,0x0000,0x0000}
#endif
;
/** @}*/







#ifdef _OBJD_
TOBJECT    OBJMEM ApplicationObjDic[] = {
/* Object 0x1600 */
{NULL , NULL ,  0x1600 , {DEFTYPE_PDOMAPPING , 255 | (OBJCODE_REC << 8)} , asEntryDesc0x1600 , aName0x1600 , &DOUnit20x1600 , NULL , NULL , 0x0000 },
/* Object 0x1601 */
{NULL , NULL ,  0x1601 , {DEFTYPE_PDOMAPPING , 255 | (OBJCODE_REC << 8)} , asEntryDesc0x1601 , aName0x1601 , &DOUnit40x1601 , NULL , NULL , 0x0000 },
/* Object 0x1A00 */
{NULL , NULL ,  0x1A00 , {DEFTYPE_PDOMAPPING , 255 | (OBJCODE_REC << 8)} , asEntryDesc0x1A00 , aName0x1A00 , &DIUnit10x1A00 , NULL , NULL , 0x0000 },
/* Object 0x1A01 */
{NULL , NULL ,  0x1A01 , {DEFTYPE_PDOMAPPING , 255 | (OBJCODE_REC << 8)} , asEntryDesc0x1A01 , aName0x1A01 , &DIUnit30x1A01 , NULL , NULL , 0x0000 },
/* Object 0x1C12 */
{NULL , NULL ,  0x1C12 , {DEFTYPE_UNSIGNED16 , 2 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C12 , aName0x1C12 , &sRxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x1C13 */
{NULL , NULL ,  0x1C13 , {DEFTYPE_UNSIGNED16 , 2 | (OBJCODE_ARR << 8)} , asEntryDesc0x1C13 , aName0x1C13 , &sTxPDOassign , NULL , NULL , 0x0000 },
/* Object 0x6000 */
{NULL , NULL ,  0x6000 , {DEFTYPE_UNSIGNED16 , 255 | (OBJCODE_ARR << 8)} , asEntryDesc0x6000 , aName0x6000 , &DIUnit10x6000 , NULL , NULL , 0x0000 },
/* Object 0x6001 */
{NULL , NULL ,  0x6001 , {DEFTYPE_UNSIGNED16 , 255 | (OBJCODE_ARR << 8)} , asEntryDesc0x6001 , aName0x6001 , &DIUnit30x6001 , NULL , NULL , 0x0000 },
/* Object 0x7000 */
{NULL , NULL ,  0x7000 , {DEFTYPE_UNSIGNED16 , 255 | (OBJCODE_ARR << 8)} , asEntryDesc0x7000 , aName0x7000 , &DOUnit20x7000 , NULL , NULL , 0x0000 },
/* Object 0x7001 */
{NULL , NULL ,  0x7001 , {DEFTYPE_UNSIGNED16 , 255 | (OBJCODE_ARR << 8)} , asEntryDesc0x7001 , aName0x7001 , &DOUnit40x7001 , NULL , NULL , 0x0000 },
/* Object 0xF008 */
{NULL , NULL ,  0xF008 , {DEFTYPE_RECORD , 4 | (OBJCODE_REC << 8)} , asEntryDesc0xF008 , aName0xF008 , &ParametersHanding0xF008 , NULL , NULL , 0x0000 },
{NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};
#endif    //#ifdef _OBJD_

#undef PROTO

/** @}*/
#define _SSC_DEVICE_OBJECTS_H_
