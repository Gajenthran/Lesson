<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/" >
<xsl:variable name="fsize" select="10"/>
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
      <p> Les médicaments "AINS" seront marqués en <span style="color:red;"> rouge </span>, les médicaments "antalgiques" seront marqués en <span style="color:orange;"> orange </span>, les "antilepreux" en <span style="color:gold">jaune doré </span> et le reste en <span style="color:green;"> vert </span>. Les doses des médicaments sont des données à vérifier donc certaines ne seront pas affichées. Les médicaments sont rangés dans l'ordre alphabétique. Concernant les conditions de prescriptions, seulement les médicaments soumis à une prescription medicale de Liste I sont indiqués sur ce tableau. </p>
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
          <th> Cond. prescription </th>
        </tr>
      
        <!-- L'ensemble des medicaments -->
        <xsl:for-each select="medicaments/medicament">
          <!-- Tri des medicaments par ordre alphabetique -->
          <xsl:sort select="nom/text()"/>
          <tr>
            <!-- position() = Récupérer la position actuelle de la valeur dans le contexte -->
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
            <xsl:choose>
              <!-- On choisit la taille de police en fonction de la longueur de la description 
               !   avec string-length -->
              <xsl:when test="string-length(description/text()) &lt; 60">
                <td style="font-size:13px"> <xsl:value-of select="description/text()"/> </td>
              </xsl:when>
              <xsl:when test="string-length(description/text()) &gt; 60">
                <td style="font-size:15px"> <xsl:value-of select="description/text()"/> </td>
              </xsl:when>
              <xsl:otherwise>
                <td style="font-size:17px"> <xsl:value-of select="description/text()"/> </td>
              </xsl:otherwise>
            </xsl:choose>

            <!-- Posologie du medicament -->
            <td> 
              <xsl:value-of select="posologie/text()"/>
              <xsl:value-of select="posologie/@unite"/> 
            </td>
            <!-- Composition du medicament (principe actif et comprimé) -->
            <td> <xsl:value-of select="composition/principe_actif/text()"/> </td>
            <td> 
              <xsl:value-of select="composition/comprime/text()"/> 
              <xsl:value-of select="composition/comprime/@unite"/> 
            </td>

            <!-- Liste des pathologies pour lesquelles ce médicament peut être prescrit -->
            <td>
                <xsl:for-each select="pathologies/pathologie">
                  <xsl:choose>
                    <!-- Si il s'agit du dernier élément de la liste, on ne rajoute pas de tiret à la fin -->
                    <xsl:when test="position() = last()">
                      <xsl:value-of select="text()"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <!-- Rajouter un tiret à chaque nouvelle pathologie (concaténation) -->
                      <xsl:value-of select="concat(text(), '- ')"/>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:for-each>
              </td>

              <!-- Vérifier les conditions de prescription et afficher seulement les liste I à l'aide 
               !   de contains -->
            <td>
              <xsl:if test="contains(condition_prescription/text(), 'II') = True ">
                <xsl:value-of select="condition_prescription/text()"/>
              </xsl:if>
            </td>
          </tr>

        </xsl:for-each>
      </table> 
    </body>
  </html>
</xsl:template>
</xsl:stylesheet>