#!/usr/bin/env bash

WORING_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

pandoc -f markdown-auto_identifiers -t latex --listings \
  -o "$WORING_DIR/../doc/latex/include/theory.tex" \
   --filter=pandoc-citeproc --bibliography="$WORING_DIR/../doc/latex/bibliography.bibtex" \
  "$WORING_DIR/../doc/README.md"
sed -i -e's+\includegraphics{images+\includegraphics[width=\\maxwidth{18cm}]{../images+g' \
  "$WORING_DIR/../doc/latex/include/theory.tex"
sed -i -e's+\[language=C\]+{C}+g' \
  "$WORING_DIR/../doc/latex/include/theory.tex"
sed -i -e's+{lstlisting}+{minted}+g' \
  "$WORING_DIR/../doc/latex/include/theory.tex"
sed -i -e's/begin{figure}/begin{figure}\[H\]/g' \
  "$WORING_DIR/../doc/latex/include/theory.tex"
sed -i -e's+maxwidth{18cm}]{../images/nrf24l01-module-standard.jpg}+maxwidth{8cm}]{../images/nrf24l01-module-standard.jpg}+g' \
  "$WORING_DIR/../doc/latex/include/theory.tex"
sed -i -e's+maxwidth{18cm}]{../images/nrf24l01-module-smd.jpg}+maxwidth{8cm}]{../images/nrf24l01-module-smd.jpg}+g' \
  "$WORING_DIR/../doc/latex/include/theory.tex"
#sed -i -e's+maxwidth{18cm}]{../images/power_consumption.jpg}+maxwidth{12cm}]{../images/power_consumption.jpg}+g' \
#  "$WORING_DIR/../doc/latex/include/theory.tex"
sed -i -e's+maxwidth{18cm}]{../images/NRF24L01-PA-LNA.jpg}+maxwidth{8cm}]{../images/NRF24L01-PA-LNA.jpg}+g' \
  "$WORING_DIR/../doc/latex/include/theory.tex"
sed -i -e's+maxwidth{18cm}]{../images/multiceiver_data_pipes.jpg}+maxwidth{12cm}]{../images/multiceiver_data_pipes.jpg}+g' \
  "$WORING_DIR/../doc/latex/include/theory.tex"

pandoc -f gfm -t latex \
  -o "$WORING_DIR/../doc/latex/include/modules/03-photoresistor.tex" \
  "$WORING_DIR/../src/Modules/03-photoresistor/README.md"
sed -i -e 's+\includegraphics{doc/images+\includegraphics[width=\\maxwidth{18cm}]{/home/spolischook/Documents/ncdu/ATP/NRF24L01/src/Modules/03-photoresistor/doc/images+g' \
  "$WORING_DIR/../doc/latex/include/modules/03-photoresistor.tex"
sed -i -e 's/Ω/$\\Omega$/g' \
  "$WORING_DIR/../doc/latex/include/modules/03-photoresistor.tex"

pandoc -f gfm -t latex \
  -o "$WORING_DIR/../doc/latex/include/modules/04-termistor.tex" \
  "$WORING_DIR/../src/Modules/04-termistor/README.md"
sed -i -e's+\includegraphics{doc/images+\includegraphics[width=\\maxwidth{18cm}]{/home/spolischook/Documents/ncdu/ATP/NRF24L01/src/Modules/04-termistor/doc/images+g' \
  "$WORING_DIR/../doc/latex/include/modules/04-termistor.tex"
sed -i -e 's/Ω/$\\Omega$/g' \
  "$WORING_DIR/../doc/latex/include/modules/04-termistor.tex"

pandoc -f gfm -t latex \
  -o "$WORING_DIR/../doc/latex/include/modules/05-ArduinoNano-RF24L01-relay.tex" \
  "$WORING_DIR/../src/Modules/05-ArduinoNano-RF24L01-relay/README.md"
sed -i -e's+\includegraphics{doc/images+\includegraphics[width=\\maxwidth{18cm}]{/home/spolischook/Documents/ncdu/ATP/NRF24L01/src/Modules/05-ArduinoNano-RF24L01-relay/doc/images+g' \
  "$WORING_DIR/../doc/latex/include/modules/05-ArduinoNano-RF24L01-relay.tex"


pandoc -f gfm -t latex \
  -o "$WORING_DIR/../doc/latex/include/modules/07-ArduinoNano-RF24L01-LCD.tex" \
  "$WORING_DIR/../src/Modules/07-ArduinoNano-RF24L01-LCD/README.md"
sed -i -e's+\includegraphics{doc/images+\includegraphics[width=\\maxwidth{18cm}]{/home/spolischook/Documents/ncdu/ATP/NRF24L01/src/Modules/07-ArduinoNano-RF24L01-LCD/doc/images+g' \
  "$WORING_DIR/../doc/latex/include/modules/07-ArduinoNano-RF24L01-LCD.tex"
