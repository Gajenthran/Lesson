<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/" >
  <html>
    <head>
      <title>Medicaments</title>
      <style>
        body {
          font-family: arial, sans-serif;
        }

        table {
          border-collapse: collapse;
          width: 100%;
        }

        tr, td, th {
          border: 1px solid #dddddd;
          text-align: center;
          padding: 8px;
        }
      </style>
    </head>
    <body>
      <!-- Titre sur la page -->
      <h1>Les medicaments : </h1>

      <!-- Description de la page -->
      <p> Les médicaments "AINS" seront marqués en <span style="color:red;"> rouge </span>, les médicaments "antalgiques" seront marqués en <span style="color:orange;"> orange </span>, les "antilepreux" en <span style="color:gold">jaune doré </span> et le reste en <span style="color:green;"> vert </span>. Les doses des médicaments sont des données à vérifier donc certaines ne seront pas affichées. Les médicaments sont rangés dans l'ordre alphabétique. </p>
      <br/>

      <table>
        <tr>
          <th> Numéro </th>
          <th> Nom </th>
          <th> Description </th>
          <th> Posologie </th>
          <th> Principe Actif </th>
          <th> Comprime </th>
          <th> Pathologies </th>
        </tr>
      
        <!-- L'ensemble des medicaments -->
        <xsl:for-each select="medicaments/medicament">
          <!-- Tri des medicaments par ordre alphabetique -->
          <xsl:sort select="nom/text()"/>
          <tr>

            <td> #<xsl:value-of select="position()"/> </td>
            <!-- Classe du medicament (couleur) -->
            <xsl:choose>
              <xsl:when test="@classe = 'AINS'">
                <td> <h3 style="color:red;"> <xsl:value-of select="nom/text()"/> </h3> </td>
              </xsl:when>
              <xsl:when test="@classe = 'Antalgique'">
                <td> <h3 style="color:orange;"> <xsl:value-of select="nom/text()"/> </h3> </td>
              </xsl:when>
              <xsl:when test="@classe = 'Antilepreux'">
                <td> <h3 style="color:gold;"> <xsl:value-of select="nom/text()"/> </h3> </td>
              </xsl:when>
              <xsl:otherwise>
                <td> <h3 style="color:green;"> <xsl:value-of select="nom/text()"/> </h3> </td>
              </xsl:otherwise>
            </xsl:choose>

            <!-- Description du medicament -->
            <td> <xsl:value-of select="description/text()"/> </td>

            <!-- Posologie du medicament -->
            <td> <xsl:value-of select="posologie/text()"/> <xsl:value-of select="posologie/@unite"/> </td>

            <!-- Composition du medicament (principe actif et comprimé) -->
            <td> <xsl:value-of select="composition/principe_actif/text()"/> </td>
            <td> <xsl:value-of select="composition/comprime/text()"/> <xsl:value-of select="posologie/@unite"/> </td>

            <!-- Liste des pathologies pour lesquelles ce médicament peut êtr prescrit -->
            <td>
              <ul>
                <xsl:for-each select="pathologies/pathologie">
                  <li> <xsl:value-of select="text()"/> </li>
                </xsl:for-each>
              </ul>
            </td>
          </tr>

        </xsl:for-each>
      </table> 
    </body>
  </html>
</xsl:template>
</xsl:stylesheet>

