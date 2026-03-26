# Audio Files

## :bulb: Description

This is the overview of specifications of the structure audio files
that will be used in this project.

## Data format of .wav file

<table border="1">
    <thead>
        <tr>
            <th>Field</th>
            <th>Size (bytes)</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>ckID</td>
            <td>4</td>
            <td>Chunk ID: fmt</td>
        </tr>
        <tr>
            <td>cksize</td>
            <td>4</td>
            <td>Chunk size: 16, 18 or 40</td>
        </tr>
        <tr>
            <td>wFormatTag</td>
            <td>2</td>
            <td>Format code</td>
        </tr>
        <tr>
            <td>nChannels</td>
            <td>2</td>
            <td>Number of interleaved channels</td>
        </tr>
        <tr>
            <td>nSamplesPerSec</td>
            <td>4</td>
            <td>Sampling rate (blocks per second)</td>
        </tr>
        <tr>
            <td>nAvgBytesPerSec</td>
            <td>4</td>
            <td>Data rate</td>
        </tr>
        <tr>
            <td>nBlockAlign</td>
            <td>2</td>
            <td>Data block size (bytes)</td>
        </tr>
        <tr>
            <td>wBitsPerSample</td>
            <td>2</td>
            <td>Bits per sample</td>
        </tr>
        <tr>
            <td>cbSize</td>
            <td>2</td>
            <td>Size of the extension (0 or 22)</td>
        </tr>
        <tr>
            <td>wValidBitsPerSample</td>
            <td>2</td>
            <td>Number of valid bits</td>
        </tr>
        <tr>
            <td>dwChannelMask</td>
            <td>4</td>
            <td>Speaker position mask</td>
        </tr>
        <tr>
            <td>SubFormat</td>
            <td>16</td>
            <td>GUID, including the data format code</td>
        </tr>
    </tbody>
</table>

## Sources

- [Specification of .wav file format.](https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html)
