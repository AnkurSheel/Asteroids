<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <!-- ******************************************************************************* -->
    <!-- The "eventType" parameter is used to filter out which events are                -->
    <!-- going to be displayed in the finall generated XHTML output                      -->
    <!--                                                                                 -->
    <!-- The following values govern specific, single, categories:                       -->
    <!-- *********************************************************                       -->
    <!-- 'Unknown'	    : Messages that didn't identify what they were                   -->
    <!-- 'Error'	    : Fatal problems with functionality                              -->
    <!-- 'Warning'	    : Recoverable problems with code                                 -->
    <!-- 'Comment'	    : General information and statistics                             -->
    <!-- 'Event'	    : Caused by a specific change in state or flow of the game       -->
    <!-- 'Debug'	    : Intermediary/Resultant data and state related information      -->
    <!-- 'Game Message' : Game play and AI related messages.                             -->
    <!--                                                                                 -->
    <!-- The following are combination options for ease of use                           -->
    <!-- *****************************************************                           -->
    <!-- 'all'	: All the above specific categories                                  -->
    <!-- 'failures'	: Only 'Error' and 'Warning'                                         -->
    <!-- 'tracking'	: 'Comment', 'Event', 'Debug' and 'Game Message'                     -->
    <!-- ******************************************************************************* -->
    <xsl:param name="eventType"				select="'all'"/>
    <!-- ******************************************************************************* -->
    <!-- The "specificFile" parameter is used to restrict the messages down to a         -->
    <!-- particular file within the C++ code base. There are no particular rules, but    -->
    <!-- for obvious reasons the filename should be correct!                             -->
    <!-- ******************************************************************************* -->
    <xsl:param name="specificFile"			select="''"/>
    <!-- This following section is the entry point for parsing the associated XML        -->
    <!-- document. It hands out parsing to the relevent subsections and generally links  -->
    <xsl:template match="/">
        <html>
            <head>
                <title>Log File Viewer</title>
            </head>
            <body>
                <font face="Arial" size="5" color="#2060AA">
                    <b>
                        <u>
                            Run-Time Log
                        </u>
                    </b>
                </font>
                <br/>
                <xsl:apply-templates select="RunTimeLog/LogHeader"/>
                <br/>
                <br/>
                <b>
                    <font face="Arial" size="3" color="#000000">
                        Details of selected log entries:
                    </font>
                </b>
                <br/>
                <font face="Arial" size="2" color="#AAAAAA">
                    This output is displaying events of type
                    <b>
                        '<xsl:copy-of select="$eventType"/>
                        '
                    </b>
                    that originated from
                    <b>
                        <xsl:copy-of select="$specificFile"/>
                    </b>
                </font>
                <br/>
                <br/>
                <table border="1" width="100%" cellspacing="0" cellpadding="0" bordercolorlight="#000000" bordercolordark="#ffffff" bordercolor="#000000">
                    <tr>
                        <td width="4%"  bgcolor="#000000">
                            <font size="2" face="Arial" color="#FFFFFF">
                                <b>
                                    <center>#</center>
                                </b>
                            </font>
                        </td>
                        <td width="14%" bgcolor="#000000">
                            <font size="2" face="Arial" color="#FFFFFF">
                                <b>
                                    <center>Time</center>
                                </b>
                            </font>
                        </td>
                        <td width="39%" bgcolor="#000000">
                            <font size="2" face="Arial" color="#FFFFFF">
                                <b>
                                    <center>File</center>
                                </b>
                            </font>
                        </td>
                        <td width="39%" bgcolor="#000000">
                            <font size="2" face="Arial" color="#FFFFFF">
                                <b>
                                    <center>Function</center>
                                </b>
                            </font>
                        </td>
                        <td width="4%"  bgcolor="#000000">
                            <font size="2" face="Arial" color="#FFFFFF">
                                <b>
                                    <center>Line</center>
                                </b>
                            </font>
                        </td>
                    </tr>
                    <xsl:apply-templates select="RunTimeLog/LogEvents/LogEvent"/>
                </table>
            </body>
        </html>
    </xsl:template>
    <!-- This next template match is for the log header, that is the general session     -->
    <!-- information and run-time system statistics/configuration                        -->
    <xsl:template match="LogHeader">
        <br/>
        <b>
            <font face="Arial" size="3" color="#000000">
                Log file header information:
            </font>
        </b>
        <br/>
        <font face="Arial" size="2" color="#000000">
            Output level:
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="OutputLevel"/>
            </font>
        </i>
        <br/>
        <font face="Arial" size="2" color="#000000">
            Session started at
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Started/Time"/>
            </font>
        </i>
        <br/>
        <font face="Arial" size="2" color="#000000">
            Operating environment:
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Configuration/Environment"/>
            </font>
        </i>
        <br/>
        <font face="Arial" size="2" color="#000000">
            System processor:
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Configuration/Processor/Family"/>
            </font>
        </i>
        <font face="Arial" size="2" color="#000000">
            running at approximately
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Configuration/Processor/ClockSpeed"/>
            </font>
        </i>
        <font face="Arial" size="2" color="#000000">
            Mhz
        </font>
        <br/>
        <font face="Arial" size="2" color="#000000">
            Physical Memory(RAM) on start-up:
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Configuration/Memory/AvailablePhysical"/>
            </font>
        </i>
        <font face="Arial" size="2" color="#000000">
            MB available from
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Configuration/Memory/TotalPhysical"/>
            </font>
        </i>
        <font face="Arial" size="2" color="#000000">
            MB installed
        </font>
        <br/>
        <font face="Arial" size="2" color="#000000">
            Virtual Memory on start-up:
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Configuration/Memory/AvailableVirtual"/>
            </font>
        </i>
        <font face="Arial" size="2" color="#000000">
            MB available from
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Configuration/Memory/TotalVirtual"/>
            </font>
        </i>
        <font face="Arial" size="2" color="#000000">
            MB installed
        </font>
        <br/>
        <font face="Arial" size="2" color="#000000">
            Hard Disk Space on start-up:
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Configuration/Memory/AvailableHardDiskSpace"/>
            </font>
        </i>
        <font face="Arial" size="2" color="#000000">
            MB available from
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:value-of select="Session/Configuration/Memory/TotalHardDiskSpace"/>
            </font>
        </i>
        <font face="Arial" size="2" color="#000000">
            MB
        </font>
        <br/>
        <font face="Arial" size="2" color="#000000">
            Total logged events:
        </font>
        <i>
            <font face="Arial" size="2" color="#808080">
                <xsl:copy-of select="count(../LogEvents/LogEvent)"/>
            </font>
        </i>
    </xsl:template>
    <!-- This template match is for each log entry, filtered according to the selected   -->
    <!-- type of event filter. For each log entry we add a row to the table.             -->
    <xsl:template match="LogEvent">
        <xsl:if test="$specificFile='' or $specificFile=File">
            <xsl:choose>
                <xsl:when test="Type='Comment'">
                    <xsl:if test="$eventType='all' or $eventType='tracking' or $eventType='Comment'">
                        <tr bgcolor="#80FF80" valign="middle">
                            <td>
                                <font size="2" face="Arial" color="#202020">
                                    <center>
                                        <xsl:value-of select="@id"/>
                                    </center>
                                </font>
                            </td>
                            <xsl:apply-templates select="TimeIndex"/>
                            <xsl:apply-templates select="File"/>
                            <xsl:apply-templates select="Function"/>
                            <xsl:apply-templates select="LineNumber"/>
                        </tr>
                        <tr bgcolor="#AAFFAA">
                            <xsl:apply-templates select="Message"/>
                        </tr>
                    </xsl:if>
                </xsl:when>
                <xsl:when test="Type='Unknown'">
                    <xsl:if test="$eventType='all' or $eventType='Unknown'">
                        <tr bgcolor="#EEEEEE" valign="middle">
                            <td>
                                <font size="2" face="Arial" color="#202020">
                                    <center>
                                        <xsl:value-of select="@id"/>
                                    </center>
                                </font>
                            </td>
                            <xsl:apply-templates select="TimeIndex"/>
                            <xsl:apply-templates select="File"/>
                            <xsl:apply-templates select="Function"/>
                            <xsl:apply-templates select="LineNumber"/>
                        </tr>
                        <tr bgcolor="#AAAAAA">
                            <xsl:apply-templates select="Message"/>
                        </tr>
                    </xsl:if>
                </xsl:when>
                <xsl:when test="Type='Error'">
                    <xsl:if test="$eventType='all' or $eventType='failures' or $eventType='Error'">
                        <tr bgcolor="#FF8080" valign="middle">
                            <td>
                                <font size="2" face="Arial" color="#202020">
                                    <center>
                                        <xsl:value-of select="@id"/>
                                    </center>
                                </font>
                            </td>
                            <xsl:apply-templates select="TimeIndex"/>
                            <xsl:apply-templates select="File"/>
                            <xsl:apply-templates select="Function"/>
                            <xsl:apply-templates select="LineNumber"/>
                        </tr>
                        <tr bgcolor="#FFAAAA">
                            <xsl:apply-templates select="Message"/>
                        </tr>
                    </xsl:if>
                </xsl:when>
                <xsl:when test="Type='Warning'">
                    <xsl:if test="$eventType='all' or $eventType='failures' or $eventType='Warning'">
                        <tr bgcolor="#FFAA80" valign="middle">
                            <td>
                                <font size="2" face="Arial" color="#202020">
                                    <center>
                                        <xsl:value-of select="@id"/>
                                    </center>
                                </font>
                            </td>
                            <xsl:apply-templates select="TimeIndex"/>
                            <xsl:apply-templates select="File"/>
                            <xsl:apply-templates select="Function"/>
                            <xsl:apply-templates select="LineNumber"/>
                        </tr>
                        <tr bgcolor="#FFDDAA">
                            <xsl:apply-templates select="Message"/>
                        </tr>
                    </xsl:if>
                </xsl:when>
                <xsl:when test="Type='Event'">
                    <xsl:if test="$eventType='all' or $eventType='tracking' or $eventType='Event'">
                        <tr bgcolor="#8080FF" valign="middle">
                            <td>
                                <font size="2" face="Arial" color="#202020">
                                    <center>
                                        <xsl:value-of select="@id"/>
                                    </center>
                                </font>
                            </td>
                            <xsl:apply-templates select="TimeIndex"/>
                            <xsl:apply-templates select="File"/>
                            <xsl:apply-templates select="Function"/>
                            <xsl:apply-templates select="LineNumber"/>
                        </tr>
                        <tr bgcolor="#AAAAFF">
                            <xsl:apply-templates select="Message"/>
                        </tr>
                    </xsl:if>
                </xsl:when>
                <xsl:when test="Type='Debug'">
                    <xsl:if test="$eventType='all' or $eventType='Debug'">
                        <tr bgcolor="#FFF880" valign="middle" align="left">
                            <td>
                                <font size="2" face="Arial" color="#202020">
                                    <center>
                                        <xsl:value-of select="@id"/>
                                    </center>
                                </font>
                            </td>
                            <xsl:apply-templates select="TimeIndex"/>
                            <xsl:apply-templates select="File"/>
                            <xsl:apply-templates select="Function"/>
                            <xsl:apply-templates select="LineNumber"/>
                        </tr>
                        <tr bgcolor="#FFFF99">
                            <xsl:apply-templates select="Message"/>
                        </tr>
                    </xsl:if>
                </xsl:when>
                <xsl:when test="Type='Game Message'">
                    <xsl:if test="$eventType='all' or $eventType='tracking' or $eventType='Game Message'">
                        <tr bgcolor="#FF8020" valign="middle">
                            <td>
                                <font size="2" face="Arial" color="#202020">
                                    <center>
                                        <xsl:value-of select="@id"/>
                                    </center>
                                </font>
                            </td>
                            <xsl:apply-templates select="TimeIndex"/>
                            <xsl:apply-templates select="File"/>
                            <xsl:apply-templates select="Function"/>
                            <xsl:apply-templates select="LineNumber"/>
                        </tr>
                        <tr bgcolor="#FFAA80">
                            <xsl:apply-templates select="Message"/>
                        </tr>
                    </xsl:if>
                </xsl:when>
            </xsl:choose>
        </xsl:if>
    </xsl:template>
    <!-- This match is for the formatted time output associate with each log entry       -->
    <xsl:template match="TimeIndex">
        <td>
            <font size="2" face="Courier New" color="#404040">
                <center>
                    <xsl:apply-templates/>
                </center>
            </font>
        </td>
    </xsl:template>
    <!-- This match is for the source code filename where this entry originated from     -->
    <xsl:template match="File">
        <td>
            <font size="2" face="Courier New" color="#404040">
                <xsl:apply-templates/>
            </font>
        </td>
    </xsl:template>
    <!-- This match outputs the qualified function name where the entry was created      -->
    <xsl:template match="Function">
        <td>
            <font size="2" face="Courier New" color="#404040">
                <xsl:apply-templates/>
            </font>
        </td>
    </xsl:template>
    <!-- This outputs the line number for the given source code file.                    -->
    <xsl:template match="LineNumber">
        <td>
            <font size="2" face="Courier New" color="#404040">
                <center>
                    <xsl:apply-templates/>
                </center>
            </font>
        </td>
    </xsl:template>
    <!-- This match outputs the raw 'Message' data that's sttached to this log entry.    -->
    <xsl:template match="Message">
        <td colspan="5">
            <font size="2" face="Arial" color="#000000">
                <b>
                    <i>
                        <xsl:apply-templates/>
                    </i>
                </b>
            </font>
        </td>
    </xsl:template>
</xsl:stylesheet>